-- MySQL Workbench Synchronization
-- Generated: 2015-06-07 17:37
-- Model: New Model
-- Version: 1.0
-- Project: Name of the project
-- Author: count0

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

ALTER TABLE `moisture`.`sensors` 
ENGINE = InnoDB ;

ALTER TABLE `moisture`.`sensor_datas` 
ENGINE = InnoDB ,
ADD COLUMN `grade_of_dryness` TINYINT(3) UNSIGNED NOT NULL AFTER `frequency`;

ALTER TABLE `moisture`.`sensor_datas` 
ADD CONSTRAINT `fk_sensor_datas_sensors`
  FOREIGN KEY (`sensor_id`)
  REFERENCES `moisture`.`sensors` (`id`)
  ON DELETE NO ACTION
  ON UPDATE NO ACTION;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
