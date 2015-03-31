CREATE DATABASE  IF NOT EXISTS `test` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `test`;
-- MySQL dump 10.13  Distrib 5.6.17, for Win32 (x86)
--
-- Host: localhost    Database: test
-- ------------------------------------------------------
-- Server version	5.6.21

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `accessories`
--

DROP TABLE IF EXISTS `accessories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `accessories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `catagoryid` varchar(45) DEFAULT NULL,
  `name` varchar(45) DEFAULT NULL,
  `price` varchar(45) DEFAULT NULL,
  `description` varchar(45) DEFAULT NULL,
  `quantity` varchar(45) DEFAULT NULL,
  `reviews` int(11) DEFAULT NULL,
  `imgpath` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=47 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accessories`
--

LOCK TABLES `accessories` WRITE;
/*!40000 ALTER TABLE `accessories` DISABLE KEYS */;
INSERT INTO `accessories` VALUES (10,'1','earring1','10','abc','30',50,'images/earring/earring1.jpg'),(11,'1','Earring2','10','Earring','30',50,'images/earring/earring2.jpg'),(12,'1','Earring3','10','Earring','30',50,'images/earring/earring3.jpg'),(13,'1','Earring4','10','Earring','30',50,'images/earring/earring4.jpg'),(14,'1','Earring5','10','Earring','30',50,'images/earring/earring5.jpg'),(15,'1','Earring6','10','Earring','30',50,'images/earring/earring6.jpg'),(16,'1','Earring7','10','Earring','30',50,'images/earring/earring7.jpg'),(17,'1','Earring8','10','Earring','30',50,'images/earring/earring8.jpg'),(18,'1','Earring9','10','Earring','30',50,'images/earring/earring9.jpg'),(19,'1','Earring10','10','Earring','30',50,'images/earring/earring10.jpg'),(20,'1','Earring11','10','Earring','30',50,'images/earring/earring11.jpg'),(21,'1','earring12','10','Earring','30',50,'images/earring/earring12.jpg'),(23,'2','Ring1','20','Ring','30',50,'images/ring/r1.jpg'),(24,'2','Ring2','20','Ring','30',50,'images/ring/r2.jpg'),(25,'2','Ring3','20','Ring','30',50,'images/ring/r3.jpg'),(26,'2','Ring4','20','Ring','30',50,'images/ring/r4.jpg'),(27,'2','Ring5','20','Ring','30',50,'images/ring/r5.jpg'),(28,'2','Ring6','20','Ring','30',50,'images/ring/r6.jpg'),(29,'2','Ring7','20','Ring','30',50,'images/ring/r7.jpg'),(30,'2','Ring8','20','Ring','30',50,'images/ring/r8.jpg'),(31,'2','Ring9','20','Ring','30',50,'images/ring/r9.jpg'),(32,'2','Ring10','20','Ring','30',50,'images/ring/r10.jpg'),(33,'2','Ring11','20','Ring','30',50,'images/ring/r11.jpg'),(34,'2','Ring12','20','Ring','30',50,'images/ring/r12.jpg'),(35,'3','Bracelet1','20','Bracelet','30',50,'images/bracelet/bracelet1.jpg'),(36,'3','Bracelet2','20','Bracelet','30',50,'images/bracelet/bracelet2.jpg'),(37,'3','Bracelet3','20','Bracelet','30',50,'images/bracelet/bracelet3.jpg'),(38,'3','Bracelet4','20','Bracelet','30',50,'images/bracelet/bracelet4.jpg'),(39,'3','Bracelet5','20','Bracelet','30',50,'images/bracelet/bracelet5.jpg'),(40,'3','Bracelet6','20','Bracelet','30',50,'images/bracelet/bracelet6.jpg'),(41,'3','Bracelet7','20','Bracelet','30',50,'images/bracelet/bracelet7.jpg'),(42,'3','Bracelet8','20','Bracelet','30',50,'images/bracelet/bracelet8.jpg'),(43,'3','Bracelet9','20','Bracelet','30',50,'images/bracelet/bracelet9.jpg'),(44,'3','Bracelet10','20','Bracelet','30',50,'images/bracelet/bracelet10.jpg'),(45,'3','Bracelet11','20','Bracelet','30',50,'images/bracelet/bracelet11.jpg'),(46,'3','Bracelet12','20','Bracelet','30',50,'images/bracelet/bracelet12.jpg');
/*!40000 ALTER TABLE `accessories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `products`
--

DROP TABLE IF EXISTS `products`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `products` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `product_code` varchar(60) NOT NULL,
  `product_name` varchar(60) NOT NULL,
  `product_desc` tinytext NOT NULL,
  `product_img_name` varchar(60) NOT NULL,
  `price` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `product_code` (`product_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `products`
--

LOCK TABLES `products` WRITE;
/*!40000 ALTER TABLE `products` DISABLE KEYS */;
/*!40000 ALTER TABLE `products` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(45) DEFAULT NULL,
  `password` varchar(45) DEFAULT NULL,
  `firstname` varchar(45) DEFAULT NULL,
  `lastname` varchar(45) DEFAULT NULL,
  `email` varchar(45) DEFAULT NULL,
  `address1` varchar(45) DEFAULT NULL,
  `address2` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (4,'asha','asha','Asha','Saini','asha.saini@gmail.com','360 Huntington Avenue','Boston, MA');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-12-10 11:49:27
