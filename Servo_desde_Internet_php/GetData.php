<?php
  $conexion = include_once "conexion.php";
  header("Content-Type: application/json");
  $data = array();
  
  //get user data from the database
  $query = $db->query("SELECT * FROM datos WHERE id = 1");
    
  while( $row = $query->fetch_assoc()){
    $data[] = $row;
  }
  
  echo json_encode($data);
?>