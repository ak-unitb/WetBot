-- MySQL Workbench Synchronization
-- Model: moisture
-- Version: 1.0
-- Project: YunFreq 
-- Author: count0

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

ALTER SCHEMA `moisture`  DEFAULT CHARACTER SET utf8  DEFAULT COLLATE utf8_unicode_ci ;

CREATE TABLE IF NOT EXISTS `moisture`.`sensors` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `pin_number` VARCHAR(2) NOT NULL,
  `name` VARCHAR(255) NOT NULL,
  `location_desciption` TEXT NULL DEFAULT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci
COMMENT = 'The sensors the aruino is connected to. just up to six senso' /* comment truncated */ /*rs are supported. The datarows need to reflect the connections to the apropriate PINs.*/;

CREATE TABLE IF NOT EXISTS `moisture`.`sensor_datas` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `frequency` INT(10) UNSIGNED NOT NULL DEFAULT 0,
  `comment` VARCHAR(255) NULL DEFAULT NULL COMMENT 'In case of error, an error description should be inserted here.',
  `sensor_id` INT(11) NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  INDEX `fk_sensor_datas_sensors_idx` (`sensor_id` ASC),
  CONSTRAINT `fk_sensor_datas_sensors`
    FOREIGN KEY (`sensor_id`)
    REFERENCES `moisture`.`sensors` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_unicode_ci
COMMENT = 'The table for analytics and statistics of the measured datas' /* comment truncated */ /*.*/;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
