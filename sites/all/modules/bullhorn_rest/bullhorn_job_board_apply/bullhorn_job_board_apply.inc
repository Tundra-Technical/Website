<?php
/**
 * Created by PhpStorm.
 * User: jnapoleone
 * Date: 12/6/2013
 * Time: 7:38 PM
 */

/**
 * Bullhorn Job Order Apply Process Job Order ID
 *
 * @param $job_order_id
 * @param $title
 * @return bool|DatabaseStatementInterface|int|null
 */

function bullhorn_job_order_apply_process_job_order_id( $job_order_id, $title ) {

    // Get the jid for the job in the {bullhorn_job_orders} database table
    $jid = db_query( 'SELECT j.jid FROM {bullhorn_job_orders} j WHERE j.job_order_id = :job_order_id', array( ':job_order_id' => $job_order_id ) )->fetchField();

    // If the job is not yet recorded, attempt save it
    if( !$jid ) {
        $jid = _job_order_save( $job_order_id,$title );
        if( !$jid ) {
            // TODO Why is this setting an error message?
//            drupal_set_message(t('A technical error occured and we were unable to complete your request. A report is being filed. Please try again later.'),'error');
            watchdog( 'bullhorn job board', 'Failed to create {bullhorn_job_orders} record for :title (:job_order_id).' , array(
                    ':title' => $title,
                    ':job_order_id'  => $job_order_id
                ),
                WATCHDOG_ERROR
            );
//            drupal_goto($return_path);
        }
    }

    return $jid;
}

/**
 * Bullhorn Job Order Apply Candidate Job Order ID Save
 *
 * Save an association between the Candidate and the JobOrders that they've applied for
 *
 * @param $jid: Local database job order id
 * @param $cid: Local candidate id
 * @param $jsid: jobSubmissionId (REST API changedEntityId)
 * @param $job_order_id: JobOrder ID
 */

function bullhorn_job_order_apply_candidate_job_order_id_save( $jid, $cid, $jsid, $job_order_id, $mess ) {

    $aid = 0;

    // If a jid exists or is created successfully create
    // a record associating it to the current Candidate
    if( $jid ) {
        $aid = _job_order_application_save ($jid, $cid, $jsid );
        if( !$aid ) {
//            drupal_set_message(t('A technical error occured and we were unable to complete your request. A report is being filed. Please try again later.'),'error');
            watchdog('bullhorn job board apply','Failed to create {bullhorn_job_order_applications} record for Candidate (:first_name :last_name) for Job Order (:title :job_order_id).',array(':first_name' => $mess['first_name'], ':last_name' => $mess['last_name'], ':title' => $mess['title'], ':job_order_id' => $job_order_id),WATCHDOG_ERROR);
//            drupal_goto($return_path);
        } else {
            // Add the Job Order ID to the current users session
            $_SESSION['bullhorn']['candidate']['applied'][] = $job_order_id;
        }
    }

    return $aid;
}

/**
 * Save the application to the database
 * @param unknown_type $jid
 *     {bullhorn_job_orders}.jid
 * @param unknown_type $cid
 *     {bullhorn_candidates}.cid
 * @param $jsid
 *     Bullhorn JobSubmissionID
 * @return Ambigous <The, NULL, number, DatabaseStatementInterface, void, string, PDOStatement>|boolean
 */
function _job_order_application_save( $jid, $cid, $jsid ) {
    try {
        $aid = db_insert( 'bullhorn_job_order_applications' )
            ->fields(array(
                'cid'  => $cid,
                'jid'  => $jid,
                'jsid' => $jsid
            ))
            ->execute();

        return $aid;
    } catch( Exception $e ) {
        //debug($e);
        watchdog('bullhorn job board','Failed to save job order application. Job Order ID (:jid), Candidate ID (:cid). Exception: :exception',array(':jid' => $jid,':cid' => $cid, ':exception' => $e));
        return false;
    }
}

/**
 * Save the Job Order to the database
 * @param int $job_order_id
 * @param string $title
 */
function _job_order_save( $job_order_id, $title ) {
    try {
        $jid = db_insert( 'bullhorn_job_orders' )
            ->fields(array(
                'job_order_id' => $job_order_id,
                'title' => $title
            ))
            ->execute();
        return $jid;
    } catch( Exception $e ) {
        //debug($e);
        watchdog('bullhorn job board','Failed to save job order :title (:job_order_id). Exception: :exception',array(':title' => $title,':job_order_id' => $job_order_id, ':exception' => $e));
        return false;
    }
}

/**
 * Sends an e-mail.
 *
 * @param $form_values
 *   An array of values from the contact form fields that were submitted.
 *   There are just two relevant items: $form_values['email'] and
 *   $form_values['message'].
 */
function bullhorn_job_board_apply_candidate_mail_send( $values ) {
    global $language;
    // All system mails need to specify the module and template key (mirrored from
    // hook_mail()) that the message they want to send comes from.
    $module = 'bullhorn_job_board_apply';
    $key = 'candidate_message';

    // Specify 'to' and 'from' addresses.
    $to = $values['email'];
    $site_name = variable_get('site_name','Tundra Technical');
    $from = $site_name . ' <noreply@tundratechnical.ca>'; //variable_get('site_mail', 'noreply@tundratechnical.ca');

    // "params" loads in additional context for email content completion in
    // hook_mail(). In this case, we want to pass in the values the user entered
    // into the form, which include the message body in $form_values['message'].
    $params = $values;
    dpm($params);


    // The language of the e-mail. This will one of three values:
    // * user_preferred_language(): Used for sending mail to a particular website
    //   user, so that the mail appears in their preferred language.
    // * global $language: Used when sending a mail back to the user currently
    //   viewing the site. This will send it in the language they're currently
    //   using.
    // * language_default(): Used when sending mail to a pre-existing, 'neutral'
    //   address, such as the system e-mail address, or when you're unsure of the
    //   language preferences of the intended recipient.
    //
    // Since in our case, we are sending a message to a random e-mail address that
    // is not necessarily tied to a user account, we will use the site's default
    // language.
//    $language = $language;

    // Whether or not to automatically send the mail when drupal_mail() is
    // called. This defaults to TRUE, and is normally what you want unless you
    // need to do additional processing before drupal_mail_send() is called.
    $send = TRUE;
    // Send the mail, and check for success. Note that this does not guarantee
    // message delivery; only that there were no PHP-related issues encountered
    // while sending.
    $result = drupal_mail( $module, $key, $to, $language, $params, $from, $send );
//    if ($result['result'] == TRUE) {
//        drupal_set_message(t('Your message has been sent.'));
//    }
//    else {
//        drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
//    }
    if($result['result'] == false) {
        watchdog('drupal_mail', "Email failed:\n@message",array('@message' => $params['body']));
    }

}

/**
 * Sends an e-mail.
 *
 * @param $values
 *   An array of values from the contact form fields that were submitted.
 *   There are just two relevant items: $form_values['email'] and
 *   $form_values['message'].
 */
function bullhorn_job_board_apply_employee_mail_send($values) {
    global $language;
    // All system mails need to specify the module and template key (mirrored from
    // hook_mail()) that the message they want to send comes from.
    $module = 'bullhorn_job_board_apply';
    $key = 'employee_message';

    // Specify 'to' and 'from' addresses.
//    $to = 'Jeff Sacco <jeffsacco@gmail.com>';//'Julian Schwaiger <jschwaiger@tundratechnical.ca>'; //$values['email'];
    $to = 'John Napoleone <john.napoleon@gmail.com>';
    $site_name = variable_get('site_name','Tundra Technical');
    $site_mail  = variable_get('site_mail','noreply@tundratechnical.ca');
    $domain_pos = (strpos($site_mail,'@')+1);
    $domain     = ($domain_pos !== FALSE) ? substr($site_mail,$domain_pos) : 'tundratechnical.ca';
    $from = $site_name . ' <noreply@'.$domain.'>'; //variable_get('site_mail', 'noreply@tundratechnical.ca');

    // "params" loads in additional context for email content completion in
    // hook_mail(). In this case, we want to pass in the values the user entered
    // into the form, which include the message body in $form_values['message'].
    $params = $values;


    // The language of the e-mail. This will one of three values:
    // * user_preferred_language(): Used for sending mail to a particular website
    //   user, so that the mail appears in their preferred language.
    // * global $language: Used when sending a mail back to the user currently
    //   viewing the site. This will send it in the language they're currently
    //   using.
    // * language_default(): Used when sending mail to a pre-existing, 'neutral'
    //   address, such as the system e-mail address, or when you're unsure of the
    //   language preferences of the intended recipient.
    //
    // Since in our case, we are sending a message to a random e-mail address that
    // is not necessarily tied to a user account, we will use the site's default
    // language.
//    $language = $language;

    // Whether or not to automatically send the mail when drupal_mail() is
    // called. This defaults to TRUE, and is normally what you want unless you
    // need to do additional processing before drupal_mail_send() is called.
    $send = TRUE;
    // Send the mail, and check for success. Note that this does not guarantee
    // message delivery; only that there were no PHP-related issues encountered
    // while sending.
    $result = drupal_mail($module, $key, $to, $language, $params, $from, $send);
//    debug($result);
//    if ($result['result'] == TRUE) {
//        drupal_set_message(t('Your message has been sent.'));
//    }
//    else {
//        drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
//    }

    if($result == false) {
        watchdog('drupal_mail', "Email failed:\n@message",array('@message' => $result['body']));
    }

}

/**
 * Implementation of hook_mail
 * @param string $key
 * @param string $message
 * @param mixed $params
 */
function bullhorn_job_board_apply_mail($key,&$message,$params) {
    // Each message is associated with a language, which may or may not be the
    // current user's selected language, depending on the type of e-mail being
    // sent. This $options array is used later in the t() calls for subject
    // and body to ensure the proper translation takes effect.
    $options = array(
        'langcode' => $message['language']->language,
    );

    switch ($key) {
        // Send a simple message from the contact form.
        case 'candidate_message':
            $message['subject'] = t('@site-name: @job_title', array('@site-name' => variable_get('site_name', 'Tundra Technical'),'@job_title' => $params['job_title']), $options);
            // Note that the message body is an array, not a string.
            //$message['body'][] = t('@name sent you the following message:', array('@name' => $user->name), $options);
            // Because this is just user-entered text, we do not need to translate it.
            $message['body'][] = t('Hello @first_name @last_name,

Thank you for applying for the position of @job_title.

Networking pays off in many ways. And at Tundra Technical Solutions, your network of great business colleagues and contacts can pay off for you in a big way. Our industry exclusive referral program, the \'Tundra Referral Incentive program\' (T.R.I.P) pays you a cash reward for helping us place your referrals.

- For every person you provide who accepts a position, you\'ll receive a $250 Cash Bonus.
- For every third successfully-placed referrals, you\'ll receive a trip to the Caribbean.
- Visit the T.R.I.P page to learn more about our program and the terms and conditions. Or click here.',array('@first_name' => $params['first_name'],'@last_name' => $params['last_name'],'@job_title' => $params['job_title']));

            // Since user-entered text may have unintentional HTML entities in it like
            // '<' or '>', we need to make sure these entities are properly escaped,
            // as the body will later be transformed from HTML to text, meaning
            // that a normal use of '<' will result in truncation of the message.
            if(isset($params['message']) && !empty($params['message'])) {
                $message['body'][] = check_plain($params['message']);
            }
            break;
        case 'employee_message':
            $message['subject'] = t('@site-name: @job_title',array('@site-name' => variable_get('site_name','Tundra Technical'),'@job_title' => $params['job_title']),$options);

            $message['body'][] = t("@first_name @last_name has applied for the position of @job_title.",array('@first_name' => $params['first_name'],'@last_name' => $params['last_name'],'@job_title' => $params['job_title']));
            if(isset($params['message']) && !empty($params['message'])) {
                $message['body'][] = check_plain($params['message']);
            }
            break;
    }
}

/**
 * Implements hook_mail_alter()
 * @param type $message
 */
function bullhorn_job_board_apply_mail_alter(&$message) {
    $site_name  = variable_get('site_name','Tundra Technical');
    $site_mail  = variable_get('site_mail','noreply@tundratechnical.ca');
    $domain_pos = (strpos($site_mail,'@')+1);
    $domain     = ($domain_pos !== FALSE) ? substr($site_mail,$domain_pos) : 'tundratechnical.ca';
    $message['headers']['Return-Path'] = $site_name . ' <noreply@'.$domain.'>';
    $message['headers']['Sender']      = $site_name . ' <noreply@'.$domain.'>';
    $message['headers']['Bcc']         = 'John Napoleone <john.napoleon@gmail.com>';
}