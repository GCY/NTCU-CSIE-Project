<?php
require('config.php');

// Set your return content type
header('Content-type: application/xml');

$request = $_POST['request'];

if(isset($request))
{
	if($request == 'ln')
		$reqUrl = '?L?N';
	if($request == 'lf')
		$reqUrl = '?L?F';
	if($request == 'mn')
		$reqUrl = '?M?N';
	if($request == 'mf')
		$reqUrl = '?M?F';
		
	echo $baseUrl.$reqUrl;
	
	// Get that website's content
	$handle = fopen($baseUrl.$reqUrl, "r");
	
	// If there is something, read and return
	if ($handle) {
	    while (!feof($handle)) {
	        $buffer = fgets($handle, 4096);
	        echo $buffer;
	    }
	    fclose($handle);
	}
}
?>