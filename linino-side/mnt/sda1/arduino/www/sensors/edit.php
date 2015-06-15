<?php
    require_once('../incs/db.php');
?>
<?php include "../incs/html_head.php" ?>
    <h1>Sensor bearbeiten</h1>
<?php
    if ($conn) {
?>
	<form action="" method="post">
<?php
        include("_form.php");
?>
	</form>
	<div>
	    <a href="index.php" class="btn btn-default">Zurück</a>
	</div>
<?php
    } else {
?>
    <p class="text-message has-error">Datenbank-Verbindung ist fehlgeschlagen.</p>
<?php
    }
?>
<?php include "../incs/html_foot.php" ?>