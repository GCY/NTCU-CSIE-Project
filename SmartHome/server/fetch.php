<?php
require('config.php');
include('mysqlPDO.php');
$pdoInstance = new mysqlPDO();
header("refresh: 10;url=$fetchUrl");

$file = trim(file_get_contents($sensorUrl));
//var_dump($file);
$data = explode("\n", $file);
var_dump($data);
$time = date("Y-m-d H:i:s");
//echo $time;

$brightness = array('data'=>$data[0], 'add_time'=>$time);
$temperature = array('data'=>$data[1], 'add_time'=>$time);
$humidity = array('data'=>$data[2], 'add_time'=>$time);
$people = array('data'=>$data[3], 'add_time'=>$time);

$pdoInstance->Add_Sql('brightness',$brightness);
$pdoInstance->Add_Sql('temperature',$temperature);
$pdoInstance->Add_Sql('humidity',$humidity);
$pdoInstance->Add_Sql('people',$people);
/*
preg_match_all('#<pre>[^<>]*</pre>#i', $file, $match);
foreach($match[0] as $value)
{
	echo $value.'<br/>';
}
*/

?>