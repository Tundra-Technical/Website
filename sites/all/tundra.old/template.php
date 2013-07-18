<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. You can add new regions for block content, modify
 *   or override Drupal's theme functions, intercept or make additional
 *   variables available to your theme, and create custom PHP logic. For more
 *   information, please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/theme-guide
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   The Drupal theme system uses special theme functions to generate HTML
 *   output automatically. Often we wish to customize this HTML output. To do
 *   this, we have to override the theme function. You have to first find the
 *   theme function that generates the output, and then "catch" it and modify it
 *   here. The easiest way to do it is to copy the original function in its
 *   entirety and paste it here, changing the prefix from theme_ to tundra_.
 *   For example:
 *
 *     original: theme_breadcrumb()
 *     theme override: tundra_breadcrumb()
 *
 *   where tundra is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_breadcrumb() function.
 *
 *   If you would like to override any of the theme functions used in Zen core,
 *   you should first look at how Zen core implements those functions:
 *     theme_breadcrumbs()      in zen/template.php
 *     theme_menu_item_link()   in zen/template.php
 *     theme_menu_local_tasks() in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called template suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node-forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and template suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440
 *   and http://drupal.org/node/190815#template-suggestions
 */


/**
 * Implementation of HOOK_theme().
 */
function tundra_theme(&$existing, $type, $theme, $path) {
  $hooks = zen_theme($existing, $type, $theme, $path);
  // Add your theme hooks like this:
  /*
  $hooks['hook_name_here'] = array( // Details go here );
  */
  // @TODO: Needs detailed comments. Patches welcome!
  return $hooks;
}

/**
 * Override or insert variables into all templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered (name of the .tpl.php file.)
 */
/* -- Delete this line if you want to use this function
function tundra_preprocess(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the page templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function tundra_preprocess_page(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // To remove a class from $classes_array, use array_diff().
  //$vars['classes_array'] = array_diff($vars['classes_array'], array('class-to-remove'));
}
// */

function tundra_preprocess_page(&$vars, $hook) {
  //print_r($vars);
  if (module_exists('domain')) {
    global $_domain;
    $vars['classes_array'][] = "domain-". strtolower( str_replace(' ','-',check_plain($_domain['sitename'])) );
    $vars['classes'] = implode(' ', $vars['classes_array']);
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function tundra_preprocess_node(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // tundra_preprocess_node_page() or tundra_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $vars['node']->type;
  if (function_exists($function)) {
    $function($vars, $hook);
  }
}
// */

function tundra_preprocess_node(&$vars,$hook){
    $node = $vars['node'];
  
  $tundra_nodes = array('14','15','16','17');
  
  if((!empty($node) && in_array($node->nid,$tundra_nodes))) { //Manage My Assignment, GŽrer Mon Assignation, Job Board, Carrires Nodes
    drupal_add_js(drupal_get_path('theme','tundra') . '/js/resize_frame.js');
  }
}

/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function tundra_preprocess_comment(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function tundra_preprocess_block(&$vars, $hook) {
  $vars['sample_variable'] = t('Lorem ipsum.');
}
// */

function tundra_preprocess_block(&$variables) {

    if($variables['block']->module == 'locale') {

        global $language;
//         dpm($language);
        
        $variables['language'] = $language->language;
        
        $languages = language_list('enabled');
//         dpm($languages);
        
        $links = array();
        foreach($languages[1] as $lang) {
            
            $links[$lang->language] = array(
                        'href'       => $_GET['q'],
                        'title'      => $lang->native,
                        'language'   => $lang,
                        'prefix'     => $lang->prefix,
                        'attributes' => array('class' => array('language-link'))
                    );
        }
        
        translation_language_switch_links_alter($links,'prefix', $_GET['q']);
        
        $variables['links'] = $links;
    }
}

/**
 * Generate the HTML output for a single menu link.
 *
 * @ingroup themeable
 */
function tundra_menu_item_link($link) {
    $menu_titles = array(
            'menu-home-page-links'
            );
    
    if (empty($link['localized_options'])) {
        $link['localized_options'] = array();
    }

    if(in_array($link['menu_name'],$menu_titles)) {
        $link['localized_options']['html'] = true;
        return l('<span class="menu-link-title">' . $link['title'] . '</span><br /><img class="menu-arrow" src="/sites/all/themes/tundra/images/arrow_on.jpg" /><br /><span class="menu-link-subtitle">' . $link['options']['attributes']['title'] . '</span>',$link['href'],$link['localized_options']);
    } else {
        return l($link['title'], $link['href'], $link['localized_options']);
    }
}