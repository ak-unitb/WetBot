<?php
    require_once('incs/db.php');
?>
<?php include "incs/html_head.php" ?>

        <h1>YunFreq - Index</h1>
<?php
    if ($conn) {
?>
        <h2>List of the last 100 saved frequency datas</h2>
<?php
        $sql = "SELECT * FROM `sensor_datas` ORDER BY `created_at` DESC LIMIT 100";

        $rs = $conn->query($sql);

        if ($rs === false) {
            trigger_error('Wrong SQL: ' . $sql . ' Error: ' . $conn->error, E_USER_ERROR);
        } else {
?>
        <table border="1">
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Frequency</th>
                    <th>Grade of Dryness</th>
                    <th>Comment</th>
                    <th>Sensor-ID</th>
                    <th>Created @</th>
                </tr>
            </thead>
            <tbody>
<?php
            $rs->data_seek(0);
            while($row = $rs->fetch_assoc()){
?>
                <tr>
                    <td><?php echo $row['id'] ?></td>
                    <td><?php echo $row['frequency'] ?></td>
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
<?php
    } else {
?>
		<h3 class="has-error">Database connection failed</h3>
<?php
    }
?>

<?php include "incs/html_foot.php" ?>