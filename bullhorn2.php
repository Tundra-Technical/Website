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
	$data = "query/JobOrder?fields=id,address,dateAdded,employmentType,skillList,description&where=id>=1&orderBy=-dateAdded&BhRestToken=".$BhRestToken;
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

print '<pre>';
print_r($joborder);
print '</pre>';


?>