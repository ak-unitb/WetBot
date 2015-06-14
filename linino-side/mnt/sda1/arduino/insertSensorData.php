<?php

    error_reporting(E_ERROR | E_PARSE);

    require_once('www/incs/db.php');

    $validArgKeys = array('frequency', 'grade_of_dryness', 'comment', 'sensor_id');

    function parseAndValidateArguments($args) {

        global $validArgKeys;

        $params = array();
        foreach ($args as $arg) {
            if (stristr($arg, '=')) {
                $argKeyValue = explode('=', $arg);
                if (count($argKeyValue) === 2) { // check if it's a valid key-value argument
                    if (in_array($argKeyValue[0], $validArgKeys)) { // check the key for validity
                        $params[$argKeyValue[0]] = $argKeyValue[1];
                    }
                }
            }
        }
        if (count($params) === count($validArgKeys)) {
            // @TODO: check the values per key for validity all needs to be numeric but comment a string...
            return $params;
        } else {
            // not enough valid arguments given:
            // we simple stop the ongoing process and return none zero.
            echo "ERROR 1";
            return array();
        }
    }


    $params = parseAndValidateArguments($argv);
    if (count($params) > 0) {
        $insertStatement = 'INSERT INTO `moisture`.`sensor_datas` (`frequency`, `grade_of_dryness`, `comment`, `sensor_id`) VALUES ('. $params['frequency'] .', '. $params['grade_of_dryness'] .', "'. $params['comment'] .'", '. $params['sensor_id'] .');';

        $rs = $conn->query($insertStatement);
        if ($rs === false) {
            // we simple stop the ongoing process and return none zero.
            echo "ERROR 2";
        } else {
            // success: return 1;
            echo "SUCCESS";
        }
    }
?>
