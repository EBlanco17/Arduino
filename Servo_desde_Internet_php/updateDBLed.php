<?php 
    $conexion = include_once "conexion.php";
    if(!empty($_POST)){
        $Led = $_POST['led'];

        //insert data
        $query = "UPDATE datos SET led = $Led WHERE id=1";
        $q = mysqli_query($conexion,$query);
        header("Location: index.php");
    }

?>