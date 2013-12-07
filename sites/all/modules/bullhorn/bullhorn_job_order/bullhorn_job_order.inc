<?php
function bullhorn_job_order_vocabularies() {
    return array(
        'job_board_location' => array(
            'name'         => 'Job Board Location',
            'machine_name' => 'job_board_location',
            'description'  => 'A vocabulary for the Job Board location Hierarchical select.',
//            'module'       => 'bullhorn job order',
        ),
        'cities' => array(
            'name'         => 'Cities',
            'machine_name' => 'cities',
            'description'  => 'A vocabulary for Bullhorn city terms.',
//            'module'       => 'bullhorn job order'
        ),
        'countries' => array(
            'name'         => 'Countries',
            'machine_name' => 'countries',
            'description'  => 'A vocabulary for Bullhorn country terms.',
//            'module'       => 'bullhorn job order'
        ),
        'job_types' => array(
            'name'         => 'Employment Types',
            'machine_name' => 'employment_types',
            'description'  => 'A vocabulary for Bullhorn employment type terms.',
//            'module'       => 'bullhorn job order',
        ),
        'categories' => array(
            'name'         => 'Categories',
            'machine_name' => 'categories',
            'description'  => 'A vocabulary for Bullhorn category terms.',
//            'module'       => 'bullhorn job order'
        ),
    );
}

/**
 * Return an array of fields to be used during the install and uninstall process
 * @return multitype:multitype:string number
 */

function bullhorn_job_order_install_fields() {
    return array(
        'field_job_order_id' => array(
            'field_name'  => 'field_job_order_id',
            'type'        => 'number_integer',
            'cardinality' => 1
        ),
        'field_rate' => array(
            'field_name'  => 'field_rate',
            'type'        => 'number_decimal',
            'cardinality' => 1
        ),
        'field_published' => array(
            'field_name'  => 'field_published',
            'type'        => 'datetime',
            'cardinality' => 1
        ),
        'field_expires' => array(
            'field_name'  => 'field_expires',
            'type'        => 'datetime',
            'cardinality' => 1
        ),
        'field_job_board_location' => array(
            'field_name'  => 'field_job_board_location',
            'type'        => 'taxonomy_term_reference',
            'cardinality' => 1,
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'job_board_location',
                        'parent' => '0',
                    ),
                ),
            ),
        ),
        'field_city' => array(
            'field_name'  => 'field_city',
            'type'        => 'taxonomy_term_reference',
            'cardinality' => 1,
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'cities',
                        'parent' => '0',
                    ),
                ),
            ),
        ),
        'field_country' => array(
            'field_name'  => 'field_country',
            'type'        => 'taxonomy_term_reference',
            'cardinality' => 1,
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'countries',
                        'parent' => '0',
                    ),
                ),
            ),
        ),
        'field_employment_type' => array(
            'field_name'  => 'field_employment_type',
            'type'        => 'taxonomy_term_reference',
            'cardinality' => 1,
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'employment_types',
                        'parent' => '0',
                    ),
                ),
            ),
        ),
        'field_category' => array(
            'field_name'  => 'field_category',
            'type'        => 'taxonomy_term_reference',
            'cardinality' => 1,
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'categories',
                        'parent' => '0',
                    ),
                ),
            ),
        ),
        'field_skills'   => array( // TODO break this comma separated list into taxonomy terms for additional searchability on website
            'field_name'  => 'field_skills',
            'type'        => 'text',
            'cardinality' => 1
        ),
    );
}

/**
 * Return an array of instances to be used during the install and uninstall process
 * @return multitype:multitype:string NULL multitype:string
 */
function bullhorn_job_order_install_instances() {
    $t = get_t();
    return array(
        'field_job_order_id' => array(
            'field_name' => 'field_job_order_id',
            'label'      => $t('Job Order ID'),
            'type'       => 'number_integer',
            'widget'     => array(
                'type' => 'number'
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
//                                                     'type'  => 'hidden',
                ),
            ),
        ),
        'field_rate' => array(
            'field_name' => 'field_rate',
            'label'      => $t('Rate'),
            'type'       => 'number_decimal',
            'widget'     => array(
                'type' => 'number'
            ),
            'settings'   => array(
                'prefix' => '$'
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'number_decimal',
                    'settings' => array(
                        'thousand_separator' => ','
                    ),
                ),
            ),
        ),
        'field_published' => array(
            'field_name' => 'field_published',
            'label'      => $t('Published'),
            'type'       => 'datetime',
            'widget'     => array(
                'type' => 'date_text'
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                ),
            ),
        ),
        'field_expires' => array(
            'field_name' => 'field_expires',
            'label'      => $t('Expires'),
            'type'       => 'datetime',
            'widget'     => array(
                'type' => 'date_text'
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                ),
            ),
        ),
        'field_job_board_location' => array(
            'field_name' => 'field_job_board_location',
            'label'      => $t('Job Board Location'),
            'type'       => 'taxonomy_term_reference',
            'widget'     => array(
                'type' => 'options_select'
            ),
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'job_board_location',
                        'parent' => '0',
                    ),
                ),
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'taxonomy_term_reference_plain'
                ),
            ),
        ),
        'field_city' => array(
            'field_name' => 'field_city',
            'label'      => $t('City'),
            'type'       => 'taxonomy_term_reference',
            'widget'     => array(
                'type' => 'options_select'
            ),
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'cities',
                        'parent' => '0',
                    ),
                ),
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'taxonomy_term_reference_plain'
                ),
            ),
        ),
        'field_country' => array(
            'field_name' => 'field_country',
            'label'      => $t('Country'),
            'type'       => 'taxonomy_term_reference',
            'widget'     => array(
                'type' => 'options_select'
            ),
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'countries',
                        'parent' => '0',
                    ),
                ),
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'taxonomy_term_reference_plain'
                ),
            ),
        ),
        'field_employment_type' => array(
            'field_name' => 'field_employment_type',
            'label'      => $t('Employment Type'),
            'type'       => 'taxonomy_term_reference',
            'widget'     => array(
                'type' => 'options_select'
            ),
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'employment_types',
                        'parent' => '0',
                    ),
                ),
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'taxonomy_term_reference_plain'
                ),
            ),
        ),
        'field_category' => array(
            'field_name' => 'field_category',
            'label'      => $t('Category'),
            'type'       => 'taxonomy_term_reference',
            'widget'     => array(
                'type' => 'options_select'
            ),
            'settings' => array(
                'allowed_values' => array(
                    array(
                        'vocabulary' => 'categories',
                        'parent' => '0',
                    ),
                ),
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                    'type'  => 'taxonomy_term_reference_plain'
                ),
            ),
        ),
        'field_skills'    => array(
            'field_name' => 'field_skills',
            'label'      => $t('Skills'),
            'type'       => 'text',
            'widget'     => array(
                'type' => 'text_textfield',
            ),
            'display'    => array(
                'default' => array(
                    'label' => 'hidden',
                ),
            ),
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