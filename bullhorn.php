<?php
$refresh_token = "2:rt-1b6e0cac-cebe-477f-b4fc-06959052b365^ac-3d4e8ef6-62ef-4ee0-9a48-0c0de1128fb6";
$client_id = "408396f4-461d-4194-b39a-4e046089ccc9";
$client_secret = "G0TfQxq5Y00khGnzrsXOmpe1HAgnVG1n";
$bhRestToken = "cb176e4f-1540-45f3-a3e8-5c82a492492c";
$bullhorn_rest_url = "https://rest2.bullhornstaffing.com/rest-services/e21i1/";
$BH_query = "query/JobOrder";
$access_token = "2:at-58bf7905-3387-400d-92da-97ceda9902fd^rt-1b6e0cac-cebe-477f-b4fc-06959052b365^ac-3d4e8ef6-62ef-4ee0-9a48-0c0de1128fb6";
$type = "job_query";
$times_loop = 0;
/**
 * State will describe what we are doing in the application
 * Types of state are [job_query, refresh_token, initial]
 * @var string
 */


$state = "initial";

/**
 * This function will set up the cURL options depending on what is needed
 * @param  [string] $type 
 * @return cURL command field
 */
function bh_curl($type)
{
	global $refresh_token, $client_secret, $client_id, $bullhorn_rest_url, $BH_query, $BhRestToken;
	$tuCurl = curl_init();

	if ($type == "job_query") 
	{
		$tuCurl == NULL;
		$BH_query_URL = $bullhorn_rest_url.$BH_query;
		$data = "?fields=id,address,dateAdded&where=id>=1&orderBy=-dateAdded&BhRestToken=".$bhRestToken;

		curl_setopt($tuCurl, CURLOPT_URL, $BH_query_URL.$data);
		curl_setopt($tuCurl, CURLOPT_PORT , 443);
		curl_setopt($tuCurl, CURLOPT_VERBOSE, 0);
		curl_setopt($tuCurl, CURLOPT_HEADER, 0);
		curl_setopt($tuCurl, CURLOPT_SSLVERSION, 3); 
		curl_setopt($tuCurl, CURLOPT_SSL_VERIFYPEER, 1); 
		curl_setopt($tuCurl, CURLOPT_RETURNTRANSFER, 1);
	}
	elseif ($type == "refresh_token") {
		$data = "grant_type=refresh_token&refresh_token=".$refresh_token."&client_id=".$client_id."&client_secret=".$client_secret;
		curl_setopt($tuCurl, CURLOPT_URL, "https://auth.bullhornstaffing.com/oauth/token");
		curl_setopt($tuCurl, CURLOPT_PORT , 443);
		curl_setopt($tuCurl, CURLOPT_VERBOSE, 0);
		curl_setopt($tuCurl, CURLOPT_HEADER, 0);
		curl_setopt($tuCurl, CURLOPT_SSLVERSION, 3);
		curl_setopt($tuCurl, CURLOPT_POST, 1);
		curl_setopt($tuCurl, CURLOPT_SSL_VERIFYPEER, 1);
		curl_setopt($tuCurl, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($tuCurl, CURLOPT_POSTFIELDS, $data);
		
	}
	else
	{
		print 'NOTHING';
	}



	return $tuCurl;
}

    function getBHAuthCode()
    {
        global $refresh_token, $client_secret, $client_id, $bullhorn_rest_url, $BH_query, $BhRestToken;
	

        $bullhornid = $client_id;
        $url = 'https://auth.bullhornstaffing.com/oauth/authorize?client_id='.$bullhornid.'&response_type=code';
        $data = "action=Login&username=tundratecapi&password=586sydney";

        $options = array(
             CURLOPT_POST           => true,
             CURLOPT_POSTFIELDS     => $data,
             CURLOPT_RETURNTRANSFER => true,
             CURLOPT_HEADER         => true,   
             CURLOPT_FOLLOWLOCATION => true,
             CURLOPT_AUTOREFERER    => true,   
             CURLOPT_CONNECTTIMEOUT => 120,
             CURLOPT_TIMEOUT        => 120,     
          );

        $ch  = curl_init( $url );
        curl_setopt_array( $ch, $options );
        $content = curl_exec( $ch );

        curl_close( $ch );
        print_r($ch);
             
        if(preg_match('#Location: (.*)#', $content, $r)) {
	       $l = trim($r[1]);
	       $temp = preg_split("/code=/", $l);
	       $authcode = $temp[1];
        }

        return $authcode;

    }

   print getBHAuthCode();
   die();


while ($times_loop < 3 || $state == "job_query" || $state != "done") {

	// Get the appropriate curl setup
	$tuCurl = bh_curl($type);

	// Make the cURL
	$tuData = curl_exec($tuCurl);

	// Check the response
	if(!curl_errno($tuCurl)){
	  $info = curl_getinfo($tuCurl);
	  echo 'Took ' . $info['total_time'] . ' seconds to send a request to ' . $info['url'];
	} else {
	  echo 'Curl error: ' . curl_error($tuCurl);
	}

	// Close down the resource
	curl_close($tuCurl);

	// Decode the response
	$raw_data = json_decode($tuData);

	$bh_curl_error = (isset($raw_data->errorMessage) ? $raw_data->errorMessage : false);

	$bh_other_error = (isset($raw_data->error) ? $raw_data->errorMessage : false);


	if ($bh_curl_error || $bh_other_error) {

		print 'We got an error.  The error is '.$raw_data->errorMessage."<br><br>";

		// Next we need to check the error to see if it has to do with the expired token.

		// If it does, we will need to repeat the process and alter the type

		if ($raw_data->errorMessage == "Bad 'BhRestToken' or timed-out.") {
			print 'We now need to get a new token<br>';
			$state = "refresh_token";
			$type = "refresh_token";
			$times_loop++;
		}
		else
		{
			// We got a regular error
		}
	}
	else
	{
		// There was no error so therefore we got through
		print '<pre>';
		print_r($tuData);
		print '</pre>';
		$state = "done";
		$times_loop++;
		

	}
	

} // END WHILE

?>