<?php
	include('login.php'); // Includes Login Script
	?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<meta http-equiv="X-UA-Compatible" content="IE=edge">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<meta name="description" content="">
		<meta name="author" content="">
		<title>Bling - The Accessory shop </title>
		<!-- Bootstrap Core CSS -->
		<link rel="stylesheet" type="text/css" href="css/style.css">
		<link href="css/bootstrap.min.css" rel="stylesheet">
		<!-- Custom CSS -->
		<link href="css/shop-homepage.css" rel="stylesheet">
		<!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
		<!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
		<!--[if lt IE 9]>
		<script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
		<script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
		<![endif]-->
	</head>
	<body class="index center">
		<div class="container">
			<div class="row">
				<div class="col-md-4 col-md-offset-7">
					<div class="panel panel-default">
						<div class="panel-heading"> <strong class="">Login</strong>
						</div>
						<div class="panel-body">
							<form class="form-horizontal" role="form" action="" method="post">
								<div class="form-group">
									<label for="username" class="col-sm-3 control-label">Username</label>
									<div class="col-sm-9">
										<input type="text" class="form-control" id="username" name="username" placeholder="Username" required="">
									</div>
								</div>
								<div class="form-group">
									<label for="password" class="col-sm-3 control-label">Password</label>
									<div class="col-sm-9">
										<input type="password" class="form-control" id="password" name="password" placeholder="Password" required="">
									</div>
								</div>
								<div class="form-group">
									<div class="col-sm-offset-3 col-sm-9">
										<div class="checkbox">
											<label class="">
											<input type="checkbox" class="">Remember me</label>
										</div>
									</div>
								</div>
								<div class="form-group last">
									<div class="col-sm-offset-3 col-sm-9">
										<input name="submit" type="submit" class="btn btn-success btn-sm" value=" Sign In">
										<button type="reset" class="btn btn-default btn-sm">Reset</button>
									</div>
								</div>
								<span><?php echo $error; ?></span>
							</form>
						</div>
						<div class="panel-footer">Not Registered? <a href="register.php" class="">Register here</a>
						</div>
					</div>
				</div>
				<form action="" method="post">
			</div>
			
		</div>
		<!-- /.container -->		
		<!-- jQuery -->
		<script src="js/jquery.js"></script>
		<!-- Bootstrap Core JavaScript -->
		<script src="js/bootstrap.min.js"></script>
	</body>
</html>