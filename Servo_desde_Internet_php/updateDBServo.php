<?php 
    $conexion = include_once "conexion.php";
    if(!empty($_POST)){
        $Servo = $_POST['servo'];

        //insert data
        $query = "UPDATE datos SET servo = $Servo WHERE id=1";
        $q = mysqli_query($conexion,$query);
        
        header("Location: index.php");
    }

?>