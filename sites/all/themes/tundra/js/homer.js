jQuery(function ($) {
  $('div#block-views-front-page-slideshow-block.block div.view div.view-content div.skin-default div#views_slideshow_cycle_main_front_page_slideshow-block.views_slideshow_cycle_main div#views_slideshow_cycle_teaser_section_front_page_slideshow-block.views-slideshow-cycle-main-frame div#views_slideshow_cycle_div_front_page_slideshow-block_0.views-slideshow-cycle-main-frame-row div.views-slideshow-cycle-main-frame-row-item div.field-content a').click(function(){
  	$.colorbox({width:"600px", height: "500px",html:"<center><h2><img src=\"/sites/default/files/tundra_trip_small.png\"></h2></center><iframe width=\"560\" height=\"315\" src=\"//www.youtube.com/embed/vwKYgjdT7fY\" frameborder=\"0\" allowfullscreen></iframe>"});
  	return false;
  });
});

jQuery(function ($) {
        
    $(window).resize(function() {
        $('.views-slideshow-cycle-main-frame').each(function(){
            var heightImgNow = '';

            $(this).find('.views-slideshow-cycle-main-frame-row').each(function(){
                    var thisDisplay = $(this).prop("style").display;
                    var thisImgHeight = $(this).find('img').height();
                    if(thisDisplay == 'block') {
                        heightImgNow = thisImgHeight;
                    }   
            });            

            if(heightImgNow != '') {
                // set div height    = now image height.
                $(this).height(heightImgNow);
            }           
        });
    });   

});
