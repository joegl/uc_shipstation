<?php
/**
 * @file
 *
 * Admin settings page.
 */

/**
 * Implements hook_admin_page().
 *
 * Provides an interface for configuring module options.
 */
function uc_shipstation_admin_page() {
  $form = array();

  // ShipStation username.
  $form['uc_shipstation_username'] = array(
    '#type' => 'textfield',
    '#title' => t('ShipStation Username'),
    '#required' => TRUE,
    '#default_value' => variable_get('uc_shipstation_username', ''),
  );

  // ShipStation password.
  $form['uc_shipstation_password'] = array(
    '#type' => 'password',
    '#title' => t('ShipStation Password'),
    '#default_value' => variable_get('uc_shipstation_password', ''),
    '#description' => t('NOTE: This field will always be blank even if a password has been saved. If left blank, the current value will remain and not be overwritten.'),
    //'#attributes' => array('autocomplete' => 'off'),
  );

  // ShipStation alternate authentication.
  $form['uc_shipstation_alternate_auth'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate Authentication'),
    '#description' => t('Use this field if your web server uses CGI to run PHP. Authorizes with an auth_key instead of the above user/pass combo.'),
    '#default_value' => variable_get('uc_shipstation_alternate_auth', ''),
  );

  // ShipStation logging.
  // @todo ShipStation: D6 Logging Support(?) D7 used WatchDog
  // $form['uc_shipstation_logging'] = array(
  //   '#type' => 'checkbox',
  //   '#title' => t('Log requests to ShipStation'),
  //   '#description' => t('If this is set, all API requests to ShipStation will be logged to Drupal watchdog.'),
  //   '#default_value' => variable_get('uc_shipstation_logging', 0),
  // );

  // ShipStation reload.
  $form['uc_shipstation_reload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reload all orders to ShipStation'),
    '#description' => t('If this is set, on API endpoint request all orders will be returned.'),
    '#default_value' => variable_get('uc_shipstation_reload', 0),
  );

  // ShipStation export paging.
  $form['uc_shipstation_export_paging'] = array(
    '#type' => 'select',
    '#title' => t('Number of Records to Export per Page'),
    '#description' => t('Sets the number of orders to send to ShipStation at a time. Change this setting if you experience import timeouts.'),
    '#options' => array(20 => 20, 50 => 50, 75 => 75, 100 => 100, 150 => 150),
    '#default_value' => variable_get('uc_shipstation_export_paging', '100'),
  );

  // Order notes to import.
  // The function below loads ALL drupal 7 fields on the site; not really a good
  // implementation in the first place.
  // For D6 we will only allow a combination of the 2 different "Comments" 
  // types as there are no "Notes" fields.
  //$notes_field_options = _uc_shipstation_load_field_options();
  $comments_field_options = array(
    'none' => 'No Comments',
    'order_only' => 'Order Comments Only',
    'admin_only' => 'Admin Comments Only',
    'order_admin' => 'Order and Admin Comments',
  );
  $form['uc_shipstation_order_comments_field'] = array(
    '#type' => 'select',
    '#title' => t('Attach Order Comments to ShipStation Orders'),
    '#required' => FALSE,
    '#description' => t('Choose any comments fields to attach to the orders'),
    '#options' => $comments_field_options,
    '#default_value' => variable_get('uc_shipstation_order_comments_field', ''),
  );

  // ShipStation order export status.
  // I believe uc_order_status_list is the d6 equivalent of below d7 function
  //$order_status_options = uc_order_status_options_list();
  $order_status_options = array();
  foreach(uc_order_status_list() as $order_status) $order_status_options[$order_status['id']] = $order_status['title'];
  $form['uc_shipstation_export_status'] = array(
    '#type' => 'select',
    '#title' => t('Order Status to Export into ShipStation'),
    '#required' => TRUE,
    '#options' => $order_status_options,
    '#default_value' => variable_get('uc_shipstation_export_status', ''),
    '#multiple' => TRUE,
  );

  // Send order state.
  $form['uc_shipstation_send_order_state'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send Order State instead of Order Status'),
    '#description' => t('ShipStation has 5 main statuses they use to organize orders whereas UberCart typically has more, based on customization. UberCart Order States tend to align better to the 5 ShipStation statuses, and potentially make it easier to organize data.'),
    '#default_value' => variable_get('uc_shipstation_send_order_state', 0),
  );

  // Order Shipping Method Exemptions.
  // Addition for D6 -- shipping method exemptions/exclusions
  $shipping_method_options = uc_quote_shipping_method_options();
  $form['uc_shipstation_shipping_method_exemptions'] = array(
    '#type' => 'select',
    '#title' => t('Shipping Method Exemptions'),
    '#description' => t('Select shipping methods which exclude the order from being pulled into ShipStation.'),
    '#options' => $shipping_method_options,
    '#default_value' => variable_get('uc_shipstation_shipping_method_exemptions', array()),
    '#multiple' => TRUE,
  );

  // add custom submit hook to keep existing password if field left blank
  // this is because password type fields default to blank (don't load in a
  // saved value) but when they are re-saved, they will overwrite the current
  // value with an empty one if the field is blank; this means everytime you
  // save this form, you have to re-enter the password or lose it.
  // using system settings form for D6 backport
  $form = system_settings_form($form);
  array_unshift($form['#submit'], 'uc_shipstation_admin_page_submit');
  return $form;  

  // $form['actions']['#type'] = 'actions';
  // $form['actions']['submit'] = array(
  //   '#type' => 'submit',
  //   '#value' => t('Save configuration'),
  // );
  //$form['#submit'][] = 'uc_shipstation_admin_page_submit';
  //return $form;
}

/**
 * Builds a list of all fields available on the site.
 */
// function _uc_shipstation_load_field_options() {
//   // WHAT? This loads ALL field definitions on the site? That's not really a 
//   // great way to do this. Maybe we just hard-code the order notes field
//   // https://api.drupal.org/api/drupal/modules%21field%21field.info.inc/function/field_info_fields/7.x
//   $options = array();
//   $options['None'] = t('None');
//   foreach (field_info_fields() as $field_name => $field) {
//     $options[$field_name] = $field_name;
//   }
//   return $options;
// }


/**
 * Custom Submit handler for the admin settings form, to keep password intact.
 *
 * add custom submit hook to keep existing password if field left blank
 * this is because password type fields default to blank (don't load in a
 * saved value) but when they are re-saved, they will overwrite the current
 * value with an empty one if the field is blank; this means everytime you
 * save this form, you have to re-enter the password or lose it.
 *
 * The D7 module used a custom submit hook for ALL the fields, but we only
 * need to use it on the password, and then system_settings_form can handle the
 * rest.
 */
function uc_shipstation_admin_page_submit($form, &$form_state) {
  // if the password is set to nothing, unset it so it doesn't overwrite current
  // value
  if($form_state['values']['uc_shipstation_password'] == '') {
    unset($form_state['values']['uc_shipstation_password']);
    drupal_set_message("Password left blank! Current value kept.");
  }
}