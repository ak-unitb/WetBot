<?php
    require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

        <h1>Statistik</h1>
<?php
    if ($conn) {

        $sql = "SELECT `sd`.*, `s`.`name` as `sensor_name` FROM `sensor_datas` sd, `sensors` s";

		$whereClause = array();
		if (!empty($_GET['grade_of_dryness'])) {
			$whereClause[] = " `sd`.`grade_of_dryness` = " . intval($_GET['grade_of_dryness']) . "";
		}
		if (!empty($_GET['comment'])) {
			$whereClause[] = " `sd`.`comment` = '" . stripslashes($_GET['comment']) . "'";
		}
		if (!empty($_GET['sensor_id'])) {
			$whereClause[] = " `sd`.`sensor_id` = " . intval($_GET['sensor_id']) . "";
		}
		
		if (!empty($whereClause)) {
			$sql .= " WHERE " . implode(' AND ', $whereClause) . " AND `sd`.`sensor_id` = `s`.`id`";
		} else {
			$sql .= " WHERE `sd`.`sensor_id` = `s`.`id`";
		}
		$sql .= " ORDER BY `sd`.`created_at` DESC LIMIT 100";

        $rs = $conn->query($sql);
        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
            $rs->data_seek(0);
			if ($rs->num_rows <= 0) {
?>
		<p class="text-message bg-warning">Keine Datensätze gefunden. Bitte prüfen Sie ggf. die FIlter-Angaben.<p>
<?php
			}
?>
		<h2>Liste der letzten 100 Einträge</h2>
        <div class="panel filterable">
        	<div class="table-responsive">
				<form action="" method="get">
		            <div class="panel-heading">
		                <h2 class="panel-title">&nbsp;</h2>
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
			                    <th>Sensor-Name</th>
			                    <th>gespeichert am/um</th>
			                </tr>
		                    <tr class="filters">
								<!--<th></th>-->
		                        <th></th>
		                        <th>
									<select class="form-control" name="grade_of_dryness" disabled>
										<option value="">-</option>
										<option value="3" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 3 ? 'selected' : ''; ?>>3</option>
										<option value="2" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 2 ? 'selected' : ''; ?>>2</option>
										<option value="1" <?php echo !empty($_GET['grade_of_dryness']) && $_GET['grade_of_dryness'] == 1 ? 'selected' : ''; ?>>1</option>
									</select>
								</th>
		                        <th><input type="text" class="form-control" name="comment" placeholder="" value="<?php echo !empty($_GET['comment']) ? $_GET['comment'] : ''; ?>" disabled></th>
		                        <th>
									<select class="form-control" name="sensor_id" disabled>
										<option value="">-</option>
										<option value="1" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 1 ? 'selected' : ''; ?>>1</option>
										<option value="2" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 2 ? 'selected' : ''; ?>>2</option>
										<option value="3" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 3 ? 'selected' : ''; ?>>3</option>
										<option value="4" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 4 ? 'selected' : ''; ?>>4</option>
										<option value="5" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 5 ? 'selected' : ''; ?>>5</option>
										<option value="6" <?php echo !empty($_GET['sensor_id']) && $_GET['sensor_id'] == 6 ? 'selected' : ''; ?>>6</option>
									</select>
								</th>
								<th><button type="submit" class="btn btn-primary pull-right">Filter anwenden</button></th>
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
			                    <td><?php echo $row['sensor_name'] ?></td>
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