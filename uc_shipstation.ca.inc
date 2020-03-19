<?php

/**
 * @file
 * This file contains the Conditional Actions hooks and functions
 */

/**
 * Implements hook_ca_trigger().
 */
function uc_shipstation_ca_trigger() {
  // order exported trigger
  $triggers['uc_shipstation_order_exported'] = array(
    '#title' => t('Order has been exported to ShipStation'),
    '#category' => t('ShipStation'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Original order'),
      ),
      // 'updated_order' => array(
      //   '#entity' => 'uc_order',
      //   '#title' => t('Updated order'),
      // ),
    ),
  );

  // order ship notified
  $triggers['uc_shipstation_order_success'] = array(
    '#title' => t('ShipStation reports an order has completed successfully'),
    '#category' => t('ShipStation'),
    '#arguments' => array(
      'order' => array(
        '#entity' => 'uc_order',
        '#title' => t('Original order'),
      ),
    ),
  );

  return $triggers;

  // $events['uc_shipstation_order_exported'] = array(
  //   'label' => t('Order has been exported to ShipStation'),
  //   'group' => t('Ubercart ShipStation'),
  //   'variables' => array(
  //     'order' => array(
  //       'label' => t('Order', array(), array('context' => 'a Drupal Ubercart order')),
  //       'type' => 'uc_order',
  //       'skip save' => TRUE,
  //     ),
  //   ),
  // );

  // $events['uc_shipstation_order_success'] = array(
  //   'label' => t('ShipStation reports an order has completed successfully'),
  //   'group' => t('Ubercart ShipStation'),
  //   'variables' => array(
  //     'order' => array(
  //       'label' => t('Order', array(), array('context' => 'a Drupal Ubercart order')),
  //       'type' => 'uc_order',
  //       'skip save' => TRUE,
  //     ),
  //     'tracking_number' => array(
  //       'type' => 'text',
  //       'label' => 'Shipping tracking number',
  //     ),
  //     'carrier' => array(
  //       'type' => 'text',
  //       'label' => 'Shipping carrier',
  //     ),
  //     'service' => array(
  //       'type' => 'text',
  //       'label' => 'Shipping service',
  //     ),
  //   ),
  // );

}


