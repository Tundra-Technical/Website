//window.onload = function (){
jQuery(document).ready(function ($) {
	header     = $("#header").outerHeight(true);
	footer     = $("#footer").outerHeight(true);
	breadcrumb = $('.breadcrumb').outerHeight(true);
	title      = $('h1.title').outerHeight(true);
	resize_iframe();
	
	function resize_iframe(){
		eFrame       = $("#frame");
		win_height   = $(window).height();
		frame_height = win_height - (header+40+breadcrumb+title+footer);
		eFrame.height(frame_height);
	}
	

	window.onresize = function (){
		resize_iframe();
	}
});
