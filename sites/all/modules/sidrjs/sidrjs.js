/**
* @file
* This file takes the menu html that was set in the .module file and invokes the sidr.js plugin
*/

(function ($) {
  
  //Variables
  var menu_output = '';

  Drupal.behaviors.sidrjs = {
    attach: function (context, settings) {
      
      //Set variables from the module
      menu_output = Drupal.settings.sidrjs.sidrjs_selected_menu_output;
      
    }
  };

jQuery(document).ready(function() {
	$(function() {
	  
	  //check if the menu is empty
	  if (menu_output || target) {
	    //wrap the menu content in <div>
	    //for the sidrjs module
	    var menu_output_render = '<div>' + menu_output + '</div>';
		//Add the Responsive menu
	    $('body').each(function() {
		  //make sure it's not the overlay
		  if (!$(this).hasClass('overlay')) {
	        $(this).prepend('<div id="mobile-header"><a id="responsive-menu-button" href="#sidr-main">Menu</a></div>');
		  }
		});  
	    
	    //Invoke Sidr
	    $('#responsive-menu-button').sidr({
		  name: 'sidr-main',
		  //the source is the sidr_target variable
		  //from the admin menu
		  source: menu_output_render
		 });
	  }
	});

});  
  
}(jQuery));