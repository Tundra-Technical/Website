/**
 * @file 
 * Calls the FooTable plugin.
 */
Drupal.behaviors.fooTable = {
  attach: function (context, settings) {
    jQuery('.footable').footable();
  }
};