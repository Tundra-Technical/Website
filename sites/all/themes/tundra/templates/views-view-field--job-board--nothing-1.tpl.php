<?php

/**
 * @file
 * This template is used to print a single field in a view.
 *
 * It is not actually used in default Views, as this is registered as a theme
 * function which has better performance. For single overrides, the template is
 * perfectly okay.
 *
 * Variables available:
 * - $view: The view object
 * - $field: The field handler object that can process the input
 * - $row: The raw SQL result that can be used
 * - $output: The processed output that will normally be used.
 *
 * When fetching output from the $row, this construct should be used:
 * $data = $row->{$field->field_alias}
 *
 * The above will guarantee that you'll always get the correct data,
 * regardless of any changes in the aliasing that might happen if
 * the view is modified.
 */
//print_r($_SESSION);
?>
<?php 
// print l('Tell a Friend','job-board/'.$row->_field_data['nid']['entity']->field_job_order_id['und'][0]['value'] .'/referral', array(
print l('Tell a Friend',$GLOBALS['base_url'].'/refer-friend?width=600&height=600&job_order='.$row->nid,
    array('attributes' => array(
        'class' => array(
           'colorbox-node',
            'view-btn',
            'tell-friend',
            ),
//         'rel'   => array(
//                 'lightmodal[|width:500px;height:500px;]'
//             ),
        )
    )
 );
?>