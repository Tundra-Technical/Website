<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
...
</head> 
<body>
<?php
define('CLIENT_ID', '408396f4-461d-4194-b39a-4e046089ccc9');
define('CLIENT_SECRET', 'G0TfQxq5Y00khGnzrsXOmpe1HAgnVG1n');
define('USER', 'tundratecapi');
define('PASS', '586sydney');

function getAuthCode()
{
$url = 'https://auth.bullhornstaffing.com/oauth/authorize?client_id='.CLIENT_ID.'&response_type=code&action=Login&username='.USER.'&password='.PASS;
$curl = curl_init( $url );
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_HEADER, true);
//curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
curl_setopt($curl, CURLOPT_AUTOREFERER, true);
curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 120);
curl_setopt($curl, CURLOPT_TIMEOUT, 120);

$content = curl_exec( $curl );
curl_close( $curl );//die($content);

if(preg_match('#Location: (.*)#', $content, $r)) {
$l = trim($r[1]);
$temp = preg_split("/code=/", $l);
$authcode = $temp[1];
}

return $authcode;
}

function doBullhornAuth($authCode)
{
$url = 'https://auth.bullhornstaffing.com/oauth/token?grant_type=authorization_code&code='.$authCode.'&client_id='.CLIENT_ID.'&client_secret='.CLIENT_SECRET;

$options = array(
CURLOPT_RETURNTRANSFER => 1,
CURLOPT_POST => true,
CURLOPT_POSTFIELDS => array()
);

$ch = curl_init( $url );
curl_setopt_array( $ch, $options );
$content = curl_exec( $ch );

curl_close( $ch ); //die($content);

return $content;

}

function doBullhornLogin($accessToken)
{
$url = 'https://rest.bullhornstaffing.com/rest-services/login?version=*&access_token='.$accessToken;
$curl = curl_init( $url );
curl_setopt($curl, CURLOPT_URL, $url);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
//curl_setopt($curl, CURLOPT_HEADER, true);
//curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
//curl_setopt($curl, CURLOPT_AUTOREFERER, true);
//curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, 120);
//curl_setopt($curl, CURLOPT_TIMEOUT, 120);

$content = curl_exec( $curl );
curl_close( $curl );
return $content;
}

function doBullhornJobQuery($BhRestToken, $BhURL)
{
	$data = "query/JobOrder?fields=title,id,address,dateAdded,employmentType,skillList,dateClosed,dateEnd,categories&where=id>=9445&orderBy=+id&BhRestToken=".$BhRestToken;
	$tuCurl = curl_init();
		curl_setopt($tuCurl, CURLOPT_URL, $BhURL.$data);
		curl_setopt($tuCurl, CURLOPT_PORT , 443);
		curl_setopt($tuCurl, CURLOPT_VERBOSE, 0);
		curl_setopt($tuCurl, CURLOPT_HEADER, 0);
		curl_setopt($tuCurl, CURLOPT_SSLVERSION, 3); 
		curl_setopt($tuCurl, CURLOPT_SSL_VERIFYPEER, 1); 
		curl_setopt($tuCurl, CURLOPT_RETURNTRANSFER, 1);
	$tuData = curl_exec($tuCurl);
	curl_close($tuCurl);

	return $tuData;
}

try {
$authCode = getAuthCode();//echo $authCode;die;
$auth = doBullhornAuth($authCode);//echo $auth;die;
$tokens = json_decode($auth);//print '<pre>';print_r($tokens);die;
$session = doBullhornLogin($tokens->access_token);

} catch (Exception $e) {
error_log($e->getMessage());
}

$jeff = json_decode($session);

$joborder = doBullhornJobQuery($jeff->BhRestToken, $jeff->restUrl);

$jeff2 = json_decode($joborder);

$cleanse = str_replace(array("\r\n","\n","\r"),'<br />',$joborder);
$num = count($jeff2->data);
//print '<pre>';
print_r('The number of responses is '.$num);
//print '</pre>';
require_once 'sites/all/libraries/htmlpurifier/library/HTMLPurifier.auto.php';

for ($i=0; $i < $num; $i++) { 
	/*
	$config = HTMLPurifier_Config::createDefault();
	$config->set('HTML.AllowedElements', 'br,ul,ol,li');  
	$config->set('Attr.AllowedClasses', '');  
	$config->set('HTML.AllowedAttributes', '');  
	$config->set('AutoFormat.RemoveEmpty', true);  
	$temp = $jeff2->data[$i]->description;
	$remarks = preg_replace('/<\?xml[^>]+\/>/im', '', $temp); 
	$purifier = new HTMLPurifier($config);
	$clean_html = $purifier->purify($remarks);
	print '<pre style="font-family: Verdana; font-size: 12px;">';
	print_r($clean_html);
	print '</pre>';
	*/
print '<pre>';
print_r($jeff2);
print '</pre>';
	}














?>


</body>
</html>