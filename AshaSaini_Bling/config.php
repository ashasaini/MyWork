<?php 
    // These variables define the connection information for your MySQL database 
    $username = "root"; 
    $password = ""; 
    $host = "localhost"; 
    $dbname = "test"; 
    
    
	$mydb = new mysqli("127.0.0.1", "root", "sa123", "test"); 
	if (mysqli_connect_errno()) {
	printf("Connect failed: %s\n", mysqli_connect_error());
	exit();
	}
    
?>