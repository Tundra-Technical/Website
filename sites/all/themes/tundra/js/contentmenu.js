/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


(function ($) {
    Drupal.behaviors.tundra = {
        attach: function(context, settings) {
            $('.content-menu').scrollToFixed({
                bottom: 0,
                limit: $('.content-menu').offset().top,
                zIndex: 499, //Keep under Drupal admin overlay
                minWidth: 1024 
            });
        }
    };
})(jQuery);