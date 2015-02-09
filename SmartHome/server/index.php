<?php
require('config.php');
include('mysqlPDO.php');
$pdoInstance = new mysqlPDO();
header("Content-Type:text/html; charset=utf-8");
header("refresh: 60;url=$indexUrl");

$sqlAmount = 50;

/* 測試用假資料 
$brit = 210;
$temp = 7;
$humi = 69;
$pirStat = 1;
$lightStat = 0;
$motorStat = 0;
*/

if($file = trim(@file_get_contents($sensorUrl)))
{
	$data = explode("\n", $file);
	//var_dump($data);
	
	$brit = $data[0];
	$temp = $data[1];
	$humi = $data[2];
	$pirStat = $data[3];
	$lightStat = $data[4];
	$motorStat = $data[5];
}
else
{
	$query = $pdoInstance->Get_Sql('select * from brightness order by id DESC limit 1', array());
	$row = $pdoInstance->fetchAll($query);
	$brit = $row[0][1];
	
	$query = $pdoInstance->Get_Sql('select * from temperature order by id DESC limit 1', array());
	$row = $pdoInstance->fetchAll($query);
	$temp = $row[0][1];
	
	$query = $pdoInstance->Get_Sql('select * from humidity order by id DESC limit 1', array());
	$row = $pdoInstance->fetchAll($query);
	$humi = $row[0][1];
	//var_dump($row[0]);
	
	$pirStat = 1;
	$lightStat = 0;
	$motorStat = 0;
}

/*
$query = $pdoInstance->Get_Sql('select * from brightness order by id DESC limit 1', array());
$row = $pdoInstance->fetchAll($query);
$lastTime = $row[0][2];
$lastBrit = $row[0][1];

$query = $pdoInstance->Get_Sql('select * from temperature order by id DESC limit 1', array());
$row = $pdoInstance->fetchAll($query);
$lastTemp = $row[0][1];

$query = $pdoInstance->Get_Sql('select * from humidity order by id DESC limit 1', array());
$row = $pdoInstance->fetchAll($query);
$lastHumi = $row[0][1];
//var_dump($row[0]);
*/

?>
<head>
	<!-- <meta http-equiv="refresh" content="1"> -->
	<title>CNP Project 智慧家電</title>
	<script type="text/javascript" src="jquery-1.9.1.min.js"></script>
	<script type="text/javascript" src="canvasjs.min.js"></script>
	
	<link href="bootstrap/css/bootstrap.css" rel="stylesheet">
	<script type="text/javascript" src="bootstrap/js/bootstrap.js"></script>
<script>

$(function(){
	var lightStatus = <?php echo $lightStat?>;
	var motorStatus = <?php echo $motorStat?>;
	var pirStatus = <?php echo $pirStat?>;

	$(".control").attr('class','btn').attr('disabled', true);
	
	if(lightStatus)
	{
		$('#lightOff').attr('class', 'control btn btn-danger').attr('disabled',false);
		$('#lightStatusWord').text('開啟').attr('class','label label-success');
	}
	else
	{
		$('#lightOn').attr('class', 'control btn btn-success').attr('disabled',false);
		$('#lightStatusWord').text('關閉').attr('class','label label-important');
	}

	if(motorStatus)
	{
		$('#motorOff').attr('class', 'control btn btn-danger').attr('disabled',false);
		$('#motorStatusWord').text('開啟').attr('class','label label-success');
	}
	else
	{
		$('#motorOn').attr('class', 'control btn btn-success').attr('disabled',false);
		$('#motorStatusWord').text('關閉').attr('class','label label-important');
	}
	
	if(pirStatus)
	{
		$('#pirStatusWord').text('有人').attr('class','label label-success');
	}
	else
	{
		$('#pirStatusWord').text('無人').attr('class','label label-important');
	}
	
	
	
	$('#lightOn').click(function(){
		$('#lightOn').attr('class', 'control btn').attr('disabled',true);
		$('#lightOff').attr('class', 'control btn btn-danger').attr('disabled',false);
		lightStatus = 1;
		send('ln');
		$('#lightStatusWord').text('開啟').attr('class','label label-success');
		//alert('開燈了');
	});
	
	$('#lightOff').click(function(){
		$('#lightOff').attr('class', 'control btn').attr('disabled',true);
		$('#lightOn').attr('class', 'control btn btn-success').attr('disabled',false);
		lightStatus = 0;
		send('lf');
		$('#lightStatusWord').text('關閉').attr('class','label label-important');
		//alert('關燈了');
	});
	
	$('#motorOn').click(function(){
		$('#motorOn').attr('class', 'control btn').attr('disabled',true);
		$('#motorOff').attr('class', 'control btn btn-danger').attr('disabled',false);
		motorStatus = 1;
		send('mn');
		$('#motorStatusWord').text('開啟').attr('class','label label-success');
		//alert('開電風扇了');
	});
	
	$('#motorOff').click(function(){
		$('#motorOff').attr('class', 'control btn').attr('disabled',true);
		$('#motorOn').attr('class', 'control btn btn-success').attr('disabled',false);
		motorStatus = 0;
		send('mf');
		$('#motorStatusWord').text('關閉').attr('class','label label-important');
		//alert('關電風扇了');
	});
	
	
	function send(control)
	{
		$.ajax({
			url: "<?php echo $actionUrl?>",
			type: "POST",
			data: {
				request: control,
			},
			success: function(){
				//alert('Test');
			},
			error: function(xhr, ajaxOptions, thrownError){
				/*
				$("body").append('<hr>');
		   		$("body").append('<br/>status:'+xhr.status+'statusText:'+xhr.statusText);
		    	$("body").append('<br/>responseText:'+xhr.responseText);
		    	$("body").append('<br/>thrownError:'+xhr.thrownError);
		    	$("body").append('<hr>');
		    	*/
		   },
		});
	
	}
});
</script>
<?php
$query = $pdoInstance->Get_Sql('select * from brightness order by id DESC limit '.$sqlAmount, array());
$britRowData = $pdoInstance->fetchAll($query);
$britRowData = array_reverse($britRowData);

$query = $pdoInstance->Get_Sql('select * from temperature order by id DESC limit '.$sqlAmount, array());
$tempRowData = $pdoInstance->fetchAll($query);
$tempRowData = array_reverse($tempRowData);

$query = $pdoInstance->Get_Sql('select * from humidity order by id DESC limit '.$sqlAmount, array());
$humiRowData = $pdoInstance->fetchAll($query);
$humiRowData = array_reverse($humiRowData);

$query = $pdoInstance->Get_Sql('select * from people order by id DESC limit 50', array());
$peoRowData = $pdoInstance->fetchAll($query);
$peoRowData = array_reverse($peoRowData);

?>
<script type="text/javascript">
  	window.onload = function () {
    	var chartBrit = new CanvasJS.Chart("chartBrightness",
    	{
	    	title:{
	      		text: "",
	      	},
	       	data: [
			{
				type: "line",
				dataPoints: [
			<?php 
				foreach($britRowData as $value)
				{
					echo '{ x: new Date("'.$value[2].'"), y: '.$value[1].' },';
				}
			?>
				//{ x: new Date("2013-06-06 17:01:11"), y: 450 },
				]
			},
	      	]
    	});
    	chartBrit.render();
    	
    	
    	
    	var chartTemp = new CanvasJS.Chart("chartTemperature",
    	{
	
	    	title:{
	      		text: "",
	      	},
	       	data: [
			{
				type: "line",
				dataPoints: [
			<?php 
				foreach($tempRowData as $value)
				{
					echo '{ x: new Date("'.$value[2].'"), y: '.$value[1].' },';
				}
			?>
				]
			},
	      	]
    	});
    	chartTemp.render();
    	
    	
    	
    	var chartHumi = new CanvasJS.Chart("chartHumidity",
    	{
	
	    	title:{
	      		text: "",
	      	},
	       	data: [
			{
				type: "line",
				dataPoints: [
			<?php 
				foreach($humiRowData as $value)
				{
					echo '{ x: new Date("'.$value[2].'"), y: '.$value[1].' },';
				}
			?>
				]
			},
	      	]
    	});
    	chartHumi.render();
    	
    	var chartPeo = new CanvasJS.Chart("chartPeople",
    	{
	
	    	title:{
	      		text: "",
	      	},
	       	data: [
			{
				type: "line",
				dataPoints: [
			<?php 
				foreach($peoRowData as $value)
				{
					echo '{ x: new Date("'.$value[2].'"), y: '.$value[1].' },';
				}
			?>
				]
			},
	      	]
    	});
    	chartPeo.render();
  	}
</script>
</head>
<body>
	<div class="navbar">
		<div class="navbar-inner">
			<a class="brand" href="<?php echo $indexUrl?>">智慧家電</a>
			<ul class="nav">
		</ul>
		</div>
	</div>
	<div class="container">
		<div class="row">
			<?php 
			/*
			<div class="span4 offset2">
				溫度記錄：<?php echo $lastTemp?>˚C<br/>
				濕度記錄：<?php echo $lastHumi?>%<br/>
				亮度記錄：<?php echo $lastBrit?><br/>
				測量時間：<?php echo $lastTime?><br/>
			</div>
			*/
			?>
			<?php
				if($temp >= 0 && $temp <= 40)
				{
					if($temp >= 0 && $temp <= 15)
						$tempStatus = '寒冷';
					if($temp > 15 && $temp <= 20)
						$tempStatus = '稍冷';
					if($temp > 20 && $temp <= 28)
						$tempStatus = '舒適';
					if($temp > 28 && $temp <= 33)
						$tempStatus = '稍熱';
					if($temp > 33 && $temp <= 40)
						$tempStatus = '炎熱';
				}
				if($humi >= 0 && $humi <= 100)
				{
					if($humi >= 0 && $humi <= 40)
						$humiStatus = '乾燥';
					if($humi > 40 && $humi <= 60)
						$humiStatus = '舒適';
					if($humi > 60 && $humi <= 100)
						$humiStatus = '潮溼';
				}
				if($brit >= 0 && $brit <= 1000)
				{
					if($brit >= 0 && $brit <= 200)
						$britStatus = '昏暗';
					if($brit > 200 && $brit <= 1000)
						$britStatus = '明亮';
				}

			?>
			<div class="span3 offset3">
				溫度狀況：<?php echo $tempStatus.'('.$temp.'˚C)'?><br/>
				濕度狀況：<?php echo $humiStatus.'('.$humi.'%)'?><br/>
				亮度狀況：<?php echo $britStatus.'('.$brit.')'?><br/>
			</div>
			<div class="span3">
				電燈：<span id="lightStatusWord" class="label"></span><br/>
				電風扇：<span id="motorStatusWord" class="label"></span><br/>
				紅外線：<span id="pirStatusWord" class="label"></span> <br/>
			</div>
		</div>
		<br/><br/>
		<div class="row">
			<div class="span3">
					人體紅外線<br/>
					<div id="chartPeople" style="height: 250px; width: 100%;"></div>
			</div>
			<div class="span3">
				亮度<br/>
				<div id="chartBrightness" style="height: 250px; width: 100%;"></div>
			</div>
			<div class="span3">
				溫度<br/>
				<div id="chartTemperature" style="height: 250px; width: 100%;"></div>
			</div>
			<div class="span3">
				濕度<br/>
				<div id="chartHumidity" style="height: 250px; width: 100%;"></div>
			</div>
		</div>	
		<br/><br/><br/>
		<div class="row">
			<div class="span3 offset3">
				<input type="button" class="control btn btn-success" id="lightOn" value="開燈">
				<input type="button" class="control btn btn-danger" id="lightOff" value="關燈">
			</div>
			<div class="span3">
				<input type="button" class="control btn" id="motorOn" value="開電風扇">
				<input type="button" class="control btn" id="motorOff" value="關電風扇">
			</div>
		</div>
	</div>
</body>