<?php
    require_once('incs/db.php');
?><!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>YunFreq - Index</title>
    <meta name="description" content="YunFreq - Index">
    <link rel="stylesheet" media="all" href="assets/css/application.css">
    <script src="assets/js/application.js"></script>
  </head>
  <body>
    <header>
      <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="/sd/">Home</a>
        </div>
        <div class="collapse navbar-collapse">
          <ul class="nav navbar-nav">
            <li><a href="#" onclick="alert('not implemented yet');return false;">Sensors</a></li>
          </ul>
        </div>
      </div>
    </nav>

    </header>
      <main role="main">

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

      </main>

  </body>
</html>
