<?php
    require_once('../incs/db.php');
?>
<?php include "../incs/html_head.php" ?>
<h1>Configured Sensors</h1>

<?php
    if ($conn) {
?>
        <h2>List of the sensors</h2>
<?php
        $sql = "SELECT * FROM `sensors` ORDER BY `id`";

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
?>
<table border="1">
	<thead>
		<th>ID</th>
		<th>Name</th>
		<th>Beschreibung</th>
		<th>Sensor-Pin-Nummer</th>
		<th>Relais-Pin-Nummer</th>
		<th>Frequenz ab dem es zu trocken ist (in Hz)</th>
		<th>Frequenz ab dem es zu nass ist (in Hz)</th>
		<th colspan="2">Aktionen</th>
	</thead>
	<tbody>
<?php
            $rs->data_seek(0);
            while($row = $rs->fetch_assoc()){
?>
		<tr>
			<td><?php echo $row['id']  ?></td>
			<td><?php echo $row['name']  ?></td>
			<td><?php echo $row['location_description']  ?></td>
			<td><?php echo $row['sensor_pin_number']  ?></td>
			<td><?php echo $row['relay_pin_numberd']  ?></td>
			<td><?php echo $row['frequency_threeshold_too_dry']  ?></td>
			<td><?php echo $row['frequency_threeshold_too_wet']  ?></td>
			<td><a href="edit.php?id=<?php echo $row['id']  ?>">Bearbeiten</a></td>
			<td><a href="?act=del&id=<?php echo $row['id']  ?>">Löschen</a></td>
		</tr>
<?php
            }
?>
	</tbody>
</table>
<?php
        }
    } else {
?>
		<h3 class="has-error">Database connection failed</h3>
<?php
    }
?>
<div>
    <a href="new.php">Neuen Sonsor konfigurieren</a>
</div>
<?php include "../incs/html_foot.php" ?>