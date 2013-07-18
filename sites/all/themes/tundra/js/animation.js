/**
 * ...
 * @author Phillip Chertok
 */

(function ($, Drupal, window, document, undefined) {
	$(function() {
		
		
		animateContentIcons();
		animateFooterItems();
		
	});
	
	function animateFooterItems()
	{
		
		var tl = new TimelineMax();
		
		$("#footer .content-menu li").css("position", "relative");
		$("#footer .content-menu li").css("opacity", 0);
		$("#footer .content-menu li").css("top", -50);
		//$(".about .content .appIcon").css("right", 50);
		
		tl.staggerTo($("#footer .content-menu li"), 0.75, {css:{opacity:1, top:0}}, 0.25);
	}
	
	function animateContentIcons()
	{
		
		var tl = new TimelineMax();
		
		$(".content .appIcon").css("opacity", 0);
		//$(".about .content .appIcon").css("right", 50);
		
		tl.staggerTo($(".content .appIcon"), 0.5, {css:{opacity:1, right:0}}, 0.5);
	}
})(jQuery, Drupal, this, this.document);