<?php

//include_once('config.php');
$dbh = 'mysql:host='.DB_HOST.';dbname='.DB_NAME;//mysql

define('DB_DSN',$dbh);
/*
	$dbh = new PDO("mysql:host=$hostname;dbname=mysql", $username, $password);//mysql
	$dbh = new PDO("odbc:Driver={Microsoft Access Driver (*.mdb)};Dbq=C:\accounts.mdb;Uid=Admin");//access
	$dbh = new PDO ("dblib:host=$hostname:$port;dbname=$dbname","$username","$password");//mssql
	$dbh = new PDO("OCI:dbname=accounts;charset=UTF-8", "username", "password");//oracle
	$dbh = new PDO("sqlite:/path/to/database.sdb");//sqlite
    
*/
class mysqlPDO{

	function __construct(){
		global $pdo;
		try{
			
			$pdo = new PDO(DB_DSN, DB_USER, DB_PASS);
			$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			$pdo->query("SET NAMES 'utf8'");
		}catch (PDOException $pdo_error){
			echo 'connect db error!',$pdo_error;
		
		}	
	}
	
	function Get_Sql($s_preSql/*語句*/,$a_con = array()/*條件句陣列*/){
		global $pdo;
		try{
			$result = $pdo->prepare($s_preSql);
			$result->execute($a_con);
			return $result;
		}catch (PDOException $pdo_error){
			echo 'query error!',$pdo_error;
		}
	}
	
	function Get_Sql_Last($s_preSql/*語句*/,$a_con = array()/*條件句陣列*/){
		global $pdo;
		try{
			$result = $pdo->prepare($s_preSql);
			$result->execute($a_con);
			return $result;
		}catch (PDOException $pdo_error){
			echo 'query error!',$pdo_error;
		}
	}
	
	function fetch($resource/*PDOstatement*/){
		try{
			return $resource->fetch(PDO::FETCH_NUM);
		}catch (PDOException $pdo_error){
			echo 'fetch error!',$pdo_error;
		}
	}
	
	function fetchall($resource/*PDOstatement*/){
		try{
			return $resource->fetchall(PDO::FETCH_NUM);
		}catch (PDOException $pdo_error){
			echo 'fetchall error!',$pdo_error;
		}
	}
	
	function Add_Sql($s_SqlTable/*資料表*/,$a_con = array()/*條件句陣列*/){
		global $pdo;
		$query = 'INSERT INTO '.$s_SqlTable.' ('.implode(',',array_keys($a_con)).') VALUES ('."'".implode("','",$a_con)."'".');';
		
		
		$result = $pdo->prepare($query);
		return $result->execute($a_con);
	}
	
	function Del_Sql($s_SqlTable/*資料表*/,$s_sqlCon/*條件句*/,$a_con=array()/*條件句陣列*/){
		global $pdo;		
		try{
			$query = 'Delete from '.$s_SqlTable.' where '.$s_sqlCon;
			//echo $query;
			$result = $pdo->prepare($query);
			return $result->execute($a_con);
		}catch (PDOException $pdo_error){
			echo 'Delete error!',$pdo_error;
		}
	}
	
	function Update_Sql($s_SqlTable/*資料表*/,$s_preSql/*條件句*/,$a_con/*條件句陣列*/,$a_data/*資料陣列*/){
		global $pdo;
		try{
			$a_con = array_values(array_merge($a_data,$a_con));
			$a_data = implode('=?,',array_keys($a_data)).'=?';
			$query = 'UPDATE '.$s_SqlTable.' SET '.$a_data.' WHERE '.$s_preSql;
			$result = $pdo->prepare($query);
			return $result->execute($a_con);
		}catch (PDOException $pdo_error){
			echo 'Update error!',$pdo_error;
		}
	}
	
	function Add_Sql2($s_SqlTable/*資料表*/,$a_con = array()/*條件句陣列*/){
		global $pdo;
		$query = 'INSERT INTO '.$s_SqlTable.' ('.implode(',',array_keys($a_con)).') VALUES ('."'".implode("','",$a_con)."'".');';
		$result = $pdo->prepare($query);
		$result->execute($a_con);
		return $pdo -> lastinsertid();
	}
};
?>