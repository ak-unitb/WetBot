<?php
    require_once('../incs/db.php');
?>
<?php include "../incs/html_head.php" ?>
    <h1>Neuen Sensor konfigurieren</h1>
<?php
    if ($conn) {
	
		$sql = "SELECT count(*) AS `countSensors` FROM `sensors`";

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
			$countSensors = 0;
            $rs->data_seek(0);
			while ($row = $rs->fetch_assoc()) {
				$countSensors = $row['countSensors'];
			}
			if ($countSensors >= 6) {
?>
	<p class="text-error bg-danger">Es können nur sechs Sensoren konfiguriert werden. Bearbeiten Sie ggf. einen bereits bestehenden.</p>
<?php
			} else {
	
?>
    <form action="" method="post">
		<input type="hidden" name="id" value="0">
<?php
        include("_form.php");
?>
    </form>
	<button type="button" class="btn btn-danger">Danger</button>
	<div>
	    <a href="index.php" class="btn btn-default">Zurück</a>
	</div>
<?php
			}
		}
    } else {
?>
    <p class="text-error bg-danger">Datenbank-Verbindung ist fehlgeschlagen.</p>
<?php
    }
?>

<?php include "../incs/html_foot.php" ?>