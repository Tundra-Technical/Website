jQuery(function ($) {
  $('div#views_slideshow_cycle_main_front_page_slideshow-block div#views_slideshow_cycle_teaser_section_front_page_slideshow-block div.views-slideshow-cycle-main-frame-row-item div.slideshow-link a').click(function(){
  	$.colorbox({html:"<center><h2><img src=\"/sites/default/files/tundra_trip_small.png\"></h2></center><iframe width=\"560\" height=\"315\" src=\"//www.youtube.com/embed/vwKYgjdT7fY\" frameborder=\"0\" allowfullscreen></iframe>"});
  	return false;
  });
});