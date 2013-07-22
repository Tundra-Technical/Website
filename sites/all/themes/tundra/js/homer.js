jQuery(function ($) {
  $('div#views_slideshow_cycle_main_front_page_slideshow-block div.views-row-0 div.slideshow-link a').click(function(){
  	$.colorbox({html:"<iframe width=\"560\" height=\"315\" src=\"//www.youtube.com/embed/vwKYgjdT7fY\" frameborder=\"0\" allowfullscreen></iframe>"});
  	return false;
  });
});