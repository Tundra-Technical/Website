<?php

/*
 * @file
 * Sidrjs Configuration Settings Form
 */

function sidrjs_config_form($form, &$form_state) {

  $form['sidrjs_menus'] = array(
	'#type' => 'checkboxes',
	'#title' => t('Menus'),
	'#default_value' => variable_get('sidrjs_menus'),
	'#options' => menu_list_system_menus(),
	'#description' => t('Select the menus you want to display in the Sidr mobile navigation.')
  );
	
  $form['sidrjs_theme'] = array(
    '#type' => 'radios',
	'#title' => t('Theme'),
	'#default_value' => variable_get('sidrjs_theme', 'dark'),
	'#options' => array(
	  'dark' => t('Dark'),
	  'light' => t('Light') 
	),
	'#description' => t('Select which style theme to use.'),
  );

  return (system_settings_form($form));
	
}
