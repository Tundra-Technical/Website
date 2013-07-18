<?php
function _job_order_vocabularies() {
    return array(
            'cities' => array(
                    'name'         => 'Cities',
                    'machine_name' => 'cities',
                    'description'  => 'A vocabulary for Bullhorn city terms.',
                    'module'       => 'bullhorn job order'
            ),
            'countries' => array(
                    'name'         => 'Countries',
                    'machine_name' => 'countries',
                    'description'  => 'A vocabulary for Bullhorn country terms.',
                    'module'       => 'bullhorn job order'
            ),
            'job_types' => array(
                    'name'         => 'Employment Types',
                    'machine_name' => 'employment_types',
                    'description'  => 'A vocabulary for Bullhorn employment type terms.',
                    'module'       => 'bullhorn job order',
            ),
            'categories' => array(
                    'name'         => 'Categories',
                    'machine_name' => 'categories',
                    'description'  => 'A vocabulary for Bullhorn category terms.',
                    'module'       => 'bullhorn job order'
            ),
    );
}

/**
 * 
 * @param object $data
 *     An object representing a single job order
 */
function _job_order_node_prepare($data) {
    global $user;
    
    $date_added = (isset($data->dateAdded) && !empty($data->dateAdded)) ? date('Y-m-d',strtotime($data->dateAdded)) : date('Y-m-d',strtotime('now'));
    $date_end   = (isset($data->dateEnd) && !empty($data->dateEnd)) ? date('Y-m-d',strtotime($data->dateEnd)) : date('Y-m-d',strtotime('+90 days'));
    
    $description = html_entity_decode($data->publicDescription,ENT_QUOTES,'UTF-8');
    
    // Tidy the description
    // TODO This is not working the way I'd like. Figure out another alternative
    // or after running Tidy create a DOM Object and iterate through the DOM
    // removing what you don't want like ALL attributes
    $tidy_config = array(
                'clean' => true,
                'bare'  => true,
                'drop-proprietary-attributes' => true,
                'output-xhtml'                => true,
                'show-body-only'              => true,
                'wrap'                        => 0,
                'indent'                      => true,
                'word-2000'                   => true
            );
    
//    $tidy = new tidy();
//    $tidy->parseString($description,$tidy_config,'');
//    $tidy->cleanRepair();
//    $body = tidy_get_output($tidy);

    $body = preg_replace('/<\?.*?\/>/', '', $description);
    
    // Create a standard Object for our node
    $node = new stdClass();
    $node->type     = 'job_order';
    
    
    node_object_prepare($node);
    $node->title    = $data->title;
    $node->language = LANGUAGE_NONE;
    
    $node->uid = $user->uid;
    
    $node->body[$node->language][0]['value']               = $body;
    $node->body[$node->language][0]['format']              = 'job_orders';
    $node->field_job_order_id[$node->language][0]['value'] = $data->jobOrderID;
    $node->field_rate[$node->language][0]['value']         = $data->payRate;
    $node->field_published[$node->language][0]['value']    = $date_added;
    $node->field_expires[$node->language][0]['value']      = $date_end;
    $node->field_skills[$node->language][0]['value']       = $data->skillList;
    
    // Set the Domain Acces for this node
    // At this time publish all JobOrder to all affiliates
    
//    $node->domains = array(1 => 1, 2 => 2, 3 => 3);
//    $node->domain_site = 1;
//    $node->domain_source = 1
    
    $city = taxonomy_get_term_by_name(ucfirst(strtolower(trim($data->address->city))),'cities');
    if(!empty($city)) {
        foreach($city as $key => $value) {
           $node->field_city[$node->language][]['tid'] = $value->tid;
        }
    }
    
    $country = taxonomy_get_term_by_name(ucfirst(strtolower(trim($data->address->country->country_name))),'countries');
    if(!empty($country)) {
        foreach($country as $value) {
            $node->field_country[$node->language][]['tid'] = $value->tid;
        }
    }
    
    $employment_type = taxonomy_get_term_by_name(ucfirst(strtolower(trim($data->employmentType))),'employment_types');
    if(!empty($employment_type)) {
        foreach($employment_type as $value){
            $node->field_employment_type[$node->language][]['tid'] = $value->tid;
        }
    }
    
    $category = taxonomy_get_term_by_name(ucfirst(strtolower(trim($data->category->name))),'categories');
    if(!empty($category)) {
        foreach($category as $value){
            $node->field_category[$node->language][]['tid'] = $value->tid;
        }
    }
    
    return $node;
}

function _job_order_node_translation_prepare($data,$lang,$default_lang) {
    // Save tnid to original node
    $node = node_load($data->nid);
    $node->tnid = $data->nid;
    $node->language = $default_lang->language;
    node_save($node);
    $node->language = $lang->language;
    $node->translate = 1;
    unset($node->nid,$node->vid);
    
    return $node;
}