/**
 * ...
 * @author Phillip Chertok
 */


function doFlashBanner()
{
	var flashvars = {
		};
		var params = {
			menu: "false",
			scale: "noScale",
			allowFullscreen: "true",
			allowScriptAccess: "always",
			bgcolor: "",
			wmode: "window" // can cause issues with FP settings & webcam
		};
		var attributes = {
			id:"tundra_banner"
		};
		swfobject.embedSWF(
			"img/flash_en-CA.swf", 
			"swfBanner", "100%", "100%", "10.0.0", 
			null, 
			flashvars, params, attributes);
			
	
}

doFlashBanner();