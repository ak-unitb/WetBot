<?php
    require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

        <h1>WetBot - Statistik</h1>
<?php
    if ($conn) {

        $sql = "SELECT * FROM `sensor_datas`";

		$whereClause = array();
		if ($_GET['grade_of_dryness']) {
			$whereClause[] = " grade_of_dryness = " . intval($_GET['grade_of_dryness']) . "";
		}
		if ($_GET['comment']) {
			$whereClause[] = " comment = " . stripslashes($_GET['comment']) . "";
		}
		if ($_GET['sensor_id']) {
			$whereClause[] = " sensor_id = " . stripslashes($_GET['comment']) . "";
		}
		
		if (!empty($whereClause)) {
			$sql .= " WHERE " . implode(' AND ', $whereClause);
		}

		$sql .= " ORDER BY `created_at` DESC LIMIT 100";

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
?>
        <div class="table-responsive">
	        <div class="panel panel-primary filterable">
				<form action="" method="get">
		            <div class="panel-heading">
		                <h2 class="panel-title">Users</h2>
		                <div class="pull-right">
		                    <button class="btn btn-default btn-xs btn-filter" type="button"><span class="glyphicon glyphicon-filter"></span> Filter</button>
		                </div>
		            </div>
			        <table class="table table-striped table-hover">
			            <thead>
			                <tr>
			                    <!--<th>ID</th>-->
			                    <th>Frequenz</th>
			                    <th>Grad der Trockenheit</th>
			                    <th>Kommentar</th>
			                    <th>Sensor-ID</th>
			                    <th>gespeichert am/um</th>
			                </tr>
		                    <tr class="filters">
								<!--<th></th>-->
		                        <th></th>
		                        <th><input type="text" class="form-control" name="grade_of_dryness" placeholder="Grad der Trockenheit" disabled></th>
		                        <th><input type="text" class="form-control" name="comment" placeholder="Kommentar" disabled></th>
		                        <th><input type="text" class="form-control" name="sensor_id" placeholder="Sensor-ID" disabled></th>
								<th></th>
		                    </tr>
			            </thead>
			            <tbody>
<?php
            $rs->data_seek(0);
            while($row = $rs->fetch_assoc()){
?>
			                <tr>
			                    <!--<td><?php echo $row['id'] ?></td>-->
			                    <td><?php echo $row['frequency'] ?> Hz</td>
			                    <td><?php echo $row['grade_of_dryness'] ?></td>
			                    <td><?php echo $row['comment'] ?></td>
			                    <td><?php echo $row['sensor_id'] ?></td>
			                    <td><?php echo $row['created_at'] ?></td>
			                </tr>
<?php
            }
        }
?>
	                	</thead>
			        </table>
				</form>
			</div>
        </div>
<?php
    } else {
?>
		<p class="text-message has-error">Datenbank Verbindung fehlgeschlagen</p>
<?php
    }
?>

<?php include "incs/html_foot.php" ?>