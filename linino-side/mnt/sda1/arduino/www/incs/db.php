<?php

    require_once('db.conf.php');

    $conn = new mysqli($DBServer, $DBUser, $DBPass, $SchemaName);

    // check connection
    if ($conn->connect_error) {
        trigger_error('Database connection failed: '  . $conn->connect_error, E_USER_ERROR);
    }

	date_default_timezone_set('Europe/Berlin');

?>
