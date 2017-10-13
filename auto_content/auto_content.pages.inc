<?php

/**
 * The Auto Content settings form
 */
function auto_content_admin_settings_form($form, &$form_state) { 

	$options_first = _auto_content_get_first_dropdown_options();

	$selected = isset($form_state['values']['auto_type_selected']) ? $form_state['values']['auto_type_selected'] : '';

	$form = array();

	$form['auto_path'] = array(
		'#type' => 'textfield',
		'#title' => t('Path to create page'),
		'#description' => t("Enter the path to create custom view page"),
		'#required' => TRUE,
	);

	$form['auto_type_selected'] = array(
		'#type' => 'select',
		'#title' => t('Select your Content Type'),
		'#options' => $options_first,
		'#required' => TRUE,
		'#ajax' => array(
			'callback' => 'auto_content_dependent_dropdown_callback',
			'wrapper' => 'dropdown-second-replace',
			'event' => 'change',
			'method' => 'replace',
			'effect' => 'fade',
		),

	);

	$form['auto_node_selected'] = array(
		'#type' => 'select',
		'#title' => t('Select your Node Content'),
		'#prefix' => '<div id="dropdown-second-replace">',
		'#suffix' => '</div>',
		'#options' => _auto_content_get_second_dropdown_options($selected),
		'#required' => TRUE,
		'#multiple' => TRUE,
	);

	$form['auto_submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
	);

	return $form;

}

/**
*
* Form Submit
**/
/**
* Auto Content From Validation and Submit
*
*/
function auto_content_admin_settings_form_submit(&$form, &$form_state) {

                $node_content_title = $form_state['values']['auto_path'];
                $node_content_type = $form_state['values']['auto_type_selected'];
                $node_content_ids = implode('#', $form_state['values']['auto_node_selected']);

                db_insert('auto_content')
                                ->fields(array('auto_content_path', 'auto_content_type', 'auto_node_type'))
                                ->values(array(
                                'auto_content_path' => $node_content_title,
                                'auto_content_type' => $node_content_type,
                                'auto_node_type' => $node_content_ids,
                ))
                ->execute();
		        drupal_set_message("successfully Saved");
                drupal_goto("/page/auto-content/".rawurlencode($form_state['values']['auto_path']));
}


/**
 * Ajax Callback Function.
 *
 */

function auto_content_dependent_dropdown_callback($form, $form_state) {
  return $form['auto_node_selected'];
}

/**
 * populate the first dropdown.
 *
 */

function _auto_content_get_first_dropdown_options(){

	$values = array();
	$results = db_select('node_type', 'n')
	    ->fields('n', array('type', 'name'))
	    ->execute()
	    ->fetchAll();

	foreach($results as $result){
	$values[$result->type] = $result->name;
	}
  return $values;
}

/**
 * populate the first dropdown.
 *
 */

function _auto_content_get_second_dropdown_options($key = '') {

	$values = array();

	$results = db_select('node', 'n')
	->condition('type', $key)
	->fields('n', array('title', 'nid','type'))
	->execute()
	->fetchAll();

	foreach($results as $result){
	$values[$result->nid] = $result->title;
	}

	if ($values != '') {
		return $values;
	}
	else {
		return array();
	}
}
