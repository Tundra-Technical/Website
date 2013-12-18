<?php
/**
 * Created by PhpStorm.
 * User: jnapoleone
 * Date: 12/5/2013
 * Time: 1:52 PM
 */

/**
 * Get Subscribed Events
 *
 * @param $session
 * @param $request_id
 * @return mixed
 */

function job_board_get_subscribed_events ( $session, $request_id=0 ) {

    $subscription_id = variable_get('bullhorn_job_board_subscription_id');

    $query = 'maxEvents=100';
    if( $request_id ) {
        $query = 'requestId=' . $request_id;
    }

    $uri = 'event/subscription/' . $subscription_id .'?' . $query . '&BhRestToken=' . $session->BhRestToken;
    $url = $session->restUrl . $uri;

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_PORT, 443);
    curl_setopt($ch, CURLOPT_VERBOSE, 0);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_SSLVERSION, 3);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $result = curl_exec($ch);

    if($result === false) {
        $ch_errno = curl_errno($ch);
        $ch_error = curl_error($ch);
        watchdog( 'bullhorn job board', 'Failed cURL request. Error No.: ( %errno ), Error: ( %error ).', array( '%errno' => $ch_errno, '%error' => $ch_error ), WATCHDOG_EMERGENCY );
        curl_close($ch);
        return false;
    }

    curl_close($ch);

//    $response = json_decode($result);

    return $result;

}

/**
 * Process Body field
 *
 * @param $body
 * @return Purified|string
 */

function job_board_clean_body_html( $body ) {

    // Require HTMLPurifier
    require_once 'sites/all/libraries/htmlpurifier/library/HTMLPurifier.auto.php';

    // Configure HTMLPurifier
    $config = HTMLPurifier_Config::createDefault();
    $config->set('HTML.AllowedElements', 'br,ul,ol,li');
    $config->set('Attr.AllowedClasses', '');
    $config->set('HTML.AllowedAttributes', '');
    $config->set('AutoFormat.RemoveEmpty', true);

    // Clean the body field
    $temp = $body;
    $remarks = preg_replace('/<\?xml[^>]+\/>/im', '', $temp);
    $purifier = new HTMLPurifier($config);
    $clean_html = $purifier->purify($remarks);
    $clean_html = nl2br($clean_html);

    return $clean_html;
}

/**
 * Process Taxonomy Term
 * Get the tid and create terms if necessary from the supplied variables
 *
 * @param $vocabulary: the machine name of the vocabulary.
 * @param $term: The term name.
 * @param bool $parent: The parent term if applicable.
 * @return array
 */

function job_board_process_taxonomy_term( $vocabulary, $term, $parent = false ) {
    $tid = '';

    if ($_term = taxonomy_get_term_by_name(ucwords(strtolower($term)), $vocabulary)) {
        $terms_array = array_keys($_term);
        $tid = $terms_array[0];
    } else {
        $_vocabulary = taxonomy_vocabulary_machine_name_load( $vocabulary );

        if($_vocabulary->vid) {

            $_term = new STDClass();
            $_term->vid = $_vocabulary->vid; //$job_board_location_vid;
            $_term->name = ucwords(strtolower( $term ));
            $_term->vocabulary_machine_name = $vocabulary;

            // If there is a parent term then set it
            if( $parent ) {
                $parent_term = taxonomy_get_term_by_name( ucwords( strtolower( $parent ) ), $vocabulary );
                $parent_term_id = array_keys( $parent_term );
                $_term->parent = $parent_term_id;
            }

            if (!empty($_term->name)) {
                $term_save = taxonomy_term_save($_term);
                $_term = taxonomy_get_term_by_name(ucwords(strtolower($term)), $vocabulary);
                $terms_array = array_keys($_term);
                $tid = $terms_array[0];
            }
        }
    }

    return $tid;
}

/**
 * Process Country ID
 * @param $id
 * @return string
 */

function job_board_process_country_id( $id ) {

    switch ( $id ) {
        case 1:
            $country = 'United States';
            break;

        case 2216:
            $country = 'Canada';
            break;

        default:
            $country = 'Canada';
    }

    return $country;
}

function job_board_get_category_name_by_id( $cat_id, $session ) {

    $uri = 'query/Category?fields=id,dateAdded,description,enabled,name,occupation,skills,specialties,type&where=id='. $cat_id .'&BhRestToken=' . $session->BhRestToken;
    $url = $session->restUrl . $uri;

    $response = bullhorn_rest_api_get_request( $url );

    return $response;
}

/**
 * Job Board Get Events By Event Type
 *
 * Create a list for handling an event type
 *
 * @param $events
 * @param $event_type
 * @return array
 */

function job_board_sort_events_by_event_type ( $events, $event_type ) {

    $_events = array();

    // Loop through the $events and create an new array for the $event_type
    foreach( $events as $event ) {
        // If the $event->entityEventType property is equal to the $event_type then push it on to the $_events Array
        if( strtolower($event->entityEventType) == strtolower($event_type) ) {
            $_events[] = $event;
        }
    }

    return $_events;
}

function job_board_count_node_by_job_order_id( $job_order_id ) {

    $query = new EntityFieldQuery();
    $query->entityCondition( 'entity_type', 'node' )
        ->entityCondition( 'bundle', 'job_order' )
        ->fieldCondition( 'field_job_order_id', 'value', $job_order_id );
    $result = $query->count()->execute();

    return $result;
}

function job_board_get_node_by_job_order_id( $job_order_id ) {

    $query = new EntityFieldQuery();
    $query->entityCondition( 'entity_type', 'node' )
        ->entityCondition( 'bundle', 'job_order' )
        ->fieldCondition( 'field_job_order_id', 'value', $job_order_id );
    $result = $query->execute();

    return $result;
}


function job_board_category_id_to_name($id)
{
    $name = '';
    switch ($id) {
        case '692418':
            $name = "applications";
            break;
        case '692451':
            $name = "engineering";
            break;
        case '692527':
            $name = "erp/sap";
            break;
        case '696836':
            $name = "infrastructure";
            break;
        case '738245':
            $name = "pmo";
            break;
        case '692511':
            $name = "other area(s)";
            break;

    }

    return $name;
}

function job_board_entity_job_order_fields_association() {
    // Set an Array of JobOrder Entity fields used on the job_order bundle
    return array(
        'title'             => 'title',
        'publicDescription' => 'body',
        'id'                => 'field_job_order_id',
        'payRate'           => 'field_rate',
        'dateAdded'         => 'field_published',
        'dateEnd'           => 'field_expires',
        'address'           => array(
            'city' => array(
                'field_city',
                'job_board_location',
            ),
            'countryID' => array(
                'field_country',
                'job_board_location',
            ),
        ),
        'employmentType'    => 'field_employment_type',
        'categories'        => 'field_category',
        'skillList'         => 'field_skills',
        'isPublic'          => 'status',
    );
}

function job_board_job_order_fields_clause(){

    $job_order_fields = job_board_entity_job_order_fields_association();

    $fields = '';

    foreach( $job_order_fields as $bullhorn_field => $job_order_field ) {
        $fields .= $bullhorn_field . ',';
    }

    if( !empty( $fields) ) {
        $fields = trim( $fields, ',' );
    }

    return $fields;
}

function job_board_delete_job_order_terms( $terms ) {
    foreach( $terms as $field => $nids ) {

        foreach( $nids as $nid => $tids ){

            foreach( $tids as $tid ){

                $query = new EntityFieldQuery();
                $query->entityCondition( 'entity_type', 'node' )
                    ->entityCondition( 'bundle', 'job_order' )
                    ->fieldCondition( $field , 'tid', $tid );
                $result = $query->count()->execute();

                if( !$result ) {
                    $deleted = taxonomy_term_delete( $tid );
                }
            }
        }
    }
}

/**
 * Job Board Test Events
 *
 * Return a hypothetical response to a REST request
 * for JobOrder INSERTED, UPDATED and DELETED events
 *
 * @return array
 */

function job_board_test_events() {

    // Set an Array of test events
    $events = (object) array(
            'requestId' => 1,
            'events'    => array(
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'updatedProperties' => array('phone'),
                    'entityEventType' => 'UPDATED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10871
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'entityEventType' => 'INSERTED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10870
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'entityEventType' => 'INSERTED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10869
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'entityEventType' => 'INSERTED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10871
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'entityEventType' => 'DELETED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10895
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'updatedProperties' => array('address'),
                    'entityEventType' => 'DELETED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10857
                ),
                (object) array(
                    'eventId' => 'ID:JBM-40004219',
                    'eventType' => 'ENTITY',
                    'entityName' => 'JobOrder',
                    'eventMetadata' => (object) array(),
                    'updatedProperties' => array('payRate','address'),
                    'entityEventType' => 'DELETED',
                    'eventTimestamp'  => 1234567890,
                    'entityId' => 10856
                )
            )
    );

    return $events;

}

/**
 * Process Job Board Location Term Field
 *
 * @param $address
 * @return array
 */

//function _bullhorn_process_job_board_location_term_field( $address ) {
//
//    // Set the address
//    $job_board_location = array();
//
//    // Set the country
//    if (strtolower($joborder_obj->data[$i]->address->countryID) == 1) {
//        $country = "United States";
//    } else if (strtolower($joborder_obj->data[$i]->address->countryID) == 2216) {
//        $country = "Canada";
//    } else {
//        $country = "Canada";
//    }
//
//    // Set the country tid and create the term if it does not exist
//    if ($term = taxonomy_get_term_by_name(ucwords(strtolower($country)), "job_board_location")) {
//        $terms_array = array_keys($term);
//        $job_board_location['country'] = $terms_array[0];
//    } else {
//        $vocabulary = taxonomy_vocabulary_machine_name_load('job_board_location');
//        $term = new STDClass();
//        $term->vid = $vocabulary->vid;
//        $term->name = ucwords(strtolower($country));
//        $term->vocabulary_machine_name = "job_board_location";
//        if (!empty($term->name)) { // Why would this be empty if it's set 2 set just above and hardcoded a little above?
//            $test = taxonomy_term_save($term);
//            $term = taxonomy_get_term_by_name(ucwords(strtolower($country)), "job_board_location");
//            $terms_array = array_keys($term);
//            $job_board_location['country'] = $terms_array[0];
//        }
//    }
//
//    // Set the city tid and create the term if it does not exist
//    if ($term = taxonomy_get_term_by_name(ucwords(strtolower($joborder_obj->data[$i]->address->city)), "job_board_location")) {
//        $terms_array = array_keys($term);
//        $job_board_location['city'] = $terms_array['0'];
//    } else {
//        $vocabulary = taxonomy_vocabulary_machine_name_load('job_board_location');
//
//        if($vocabulary->vid) {
//
//            $parent_term = taxonomy_get_term_by_name( ucwords( strtolower( $country ) ), 'job_board_location' );
//            $parent_term_id = array_keys( $parent_term );
//
//            $term = new STDClass();
//            $term->vid = $vocabulary->vid; //$job_board_location_vid;
//            $term->name = ucwords(strtolower($joborder_obj->data[$i]->address->city));
//            $term->parent = $parent_term_id;
//            $term->vocabulary_machine_name = "job_board_location";
//            if (!empty($term->name)) {
//                $term_save = taxonomy_term_save($term);
//                $term = taxonomy_get_term_by_name(ucwords(strtolower($joborder_obj->data[$i]->address->city)), "job_board_location");
//                $terms_array = array_keys($term);
//                $job_board_location['city'] = $terms_array[0];
//            }
//        }
//    }
//
//    return $job_board_location;
//}