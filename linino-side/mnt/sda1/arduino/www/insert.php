<?php

require_once('incs/db.php');

$text = "";

if (!empty($_GET['text'])) {
	$text = $_GET['text'];
}

if (empty($text)) {
	trigger_error('No text for the next database entry given!', E_USER_ERROR);
}

if ($conn) {

	$text = "'" . $conn->real_escape_string($text) . "'";

	$sql = "INSERT INTO `example` (`data`) VALUES($text)";

	$rs = $conn->query($sql);

	if($rs === false) {
		trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
	} else {
		echo "Another row inserted!<br>";
	}

	$sql = "SELECT * FROM `example`";

	$rs = $conn->query($sql);

	if ($rs === false) {
		trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
	} else {

		$rs->data_seek(0);
		while($row = $rs->fetch_assoc()){
			echo $row['data'] . '<br>';
		}

	}

}

?>
