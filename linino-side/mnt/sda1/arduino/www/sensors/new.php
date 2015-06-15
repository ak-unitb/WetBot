<?php
    require_once('../incs/db.php');
?>
<?php include "../incs/html_head.php" ?>
    <h1>Create new Sensor</h1>
<?php
    if ($conn) {
?>
	<form action="" method="get">
		<?php include("_form.php"); ?>
	</form>
<?php
    } else {
?>
    <h3 class="has-error">Database connection failed</h3>
<?php
    }
?>
<div>
    <a href="index.php">Zurück</a>
</div>
<?php include "../incs/html_foot.php" ?>