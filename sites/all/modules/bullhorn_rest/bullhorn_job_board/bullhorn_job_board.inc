<?php
/**
 * Created by PhpStorm.
 * User: jnapoleone
 * Date: 12/5/2013
 * Time: 1:52 PM
 */

/**
 * Bullhorn Get Subscribed Events
 *
 * @param $session
 * @return mixed
 */

function _bullhorn_get_subscribed_events ( $session ) {

    $subscription_id = variable_get('bullhorn_rest_api_subscription_id');

    $uri = 'event/subscription/' . $subscription_id .'?maxEvents=100&BhRestToken=' . $session->BhRestToken;
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
//        dpm($ch_errno,'Error number');
//        dpm($ch_error,'Error message');
//        dpm(curl_getinfo(($ch)), 'cURL get information');
        curl_close($ch);
    }

    curl_close($ch);

    $response = json_decode($result);

    return $response;

}

/**
 * Bullhorn Process Body field
 *
 * @param $body
 * @return Purified|string
 */

function _bullhorn_process_body_field( $body ) {

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
 * Bullhorn Process Taxonomy Term
 * Get the tid and create terms if necessary from the supplied variables
 *
 * @param $vocabulary: the machine name of the vocabulary.
 * @param $term: The term name.
 * @param bool $parent: The parent term if applicable.
 * @return array
 */

function _bullhorn_process_taxonomy_term( $vocabulary, $term, $parent = false ) {
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

//    dpm($tid);

    return $tid;
}

/**
 * Bullhorn Process Country ID
 * @param $id
 * @return string
 */

function _bullhorn_process_country_id( $id ) {

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

function _bullhorn_get_category_name_by_id( $cat_id, $session ) {

    $uri = 'query/Category?fields=id,dateAdded,description,enabled,name,occupation,skills,specialties,type&where=id='. $cat_id .'&BhRestToken=' . $session->BhRestToken;
    $url = $session->restUrl . $uri;

    $response = bullhorn_rest_api_get_request( $url );

//    dpm($response);

    return $response;
}

/**
 * Bullhorn Process Job Board Location Term Field
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