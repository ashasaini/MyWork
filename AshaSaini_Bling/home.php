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
	<body class="center">
		<!-- Navigation -->
		<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
			<div class="container">
				<!-- Brand and toggle get grouped for better mobile display -->
				<div class="navbar-header">
					<button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
					<span class="sr-only">Toggle navigation</span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					<span class="icon-bar"></span>
					</button>
					<a class="navbar-brand" href="home.php">Bling</a>	<!-- pending -->
				</div>
				<!-- Collect the nav links, forms, and other content for toggling -->
				<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
					<ul class="nav navbar-nav">
						<li>
							<a href="about.php">About</a>	<!-- pending -->
						</li>
						<li>
							<a href="accessories.php">Accessories</a> <!-- pending -->
						</li>
						<li>
							<a href="contact.php">Contact US</a> <!-- pending -->
						</li>
						<li>
							<a href="cart.php"  class="glyphicon glyphicon-shopping-cart">Shopping Cart</a> <!-- pending -->
						</li>
						<li>
							<a href="logout.php">Logout</a> <!-- pending -->
						</li>
						<!-- <li>					
						<input type="text" class="input-medium search-query" name="s" placeholder="Search" value="">
						<button type="submit" class="add-on"><i class="icon-search"></i></button>
					    </li> -->
						 <div class="col-sm-3 col-md-3">
						<form class="navbar-form" role="search">
						<div class="input-group">
						<input type="text" class="form-control" placeholder="Search" name="q">
						<div class="input-group-btn">
						<button class="btn btn-default" type="submit"><i class="glyphicon glyphicon-search"></i></button>
						</div>
						</div>
						</form>						
    </div>
					</ul>
				</div>
				<!-- /.navbar-collapse -->
			</div>
			<!-- /.container -->
		</nav>
		<!-- Page Content -->
		<div class="container">
			<div class="row">
				<div class="col-md-9">
					<div class="row carousel-holder">
						<div class="col-md-12">
							<div id="carousel-example-generic" class="carousel slide" data-ride="carousel">
								<ol class="carousel-indicators">
									<li data-target="#carousel-example-generic" data-slide-to="0" class="active"></li>
									<li data-target="#carousel-example-generic" data-slide-to="1"></li>
									<li data-target="#carousel-example-generic" data-slide-to="2"></li>
								</ol>
								<div class="carousel-inner">
									<div class="item active">
										<img class="slide-image" src="images/carousel/slide1.jpg" alt="">
									</div>
									<div class="item">
										<img class="slide-image" src="images/carousel/slide2.jpg" alt="">
									</div>
									<div class="item">
										<img class="slide-image" src="images/carousel/slide3.jpg" alt="">
									</div>
								</div>
								<a class="left carousel-control" href="#carousel-example-generic" data-slide="prev">
								<span class="glyphicon glyphicon-chevron-left"></span>
								</a>
								<a class="right carousel-control" href="#carousel-example-generic" data-slide="next">
								<span class="glyphicon glyphicon-chevron-right"></span>
								</a>
							</div>
						</div>
					</div>
					<div class="row">
						<div class="form-group last">
							<div class="col-sm-offset-3 col-sm-9">
								<a href="accessories.php" class="btn btn-success btn-lg active pull-right" role="button">Shop Now >></a>								
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
		</br>
		</br>
		<!-- /.container -->
		<div class="container">
			<!-- Footer -->
			<link href="//maxcdn.bootstrapcdn.com/font-awesome/4.2.0/css/font-awesome.min.css" rel="stylesheet">
			<div class="footer" id="footer">
				<div class="container">
					<div class="row">
						<div class="col-lg-2  col-md-2 col-sm-4 col-xs-6">
							<h4> COMPANY </h4>
							<ul>
								<li> <a href="#"> About Us </a> </li>
								<li> <a href="#"> Careers </a> </li>
								<li> <a href="#"> Newsroom </a> </li>
								<li> <a href="#"> Business with us </a> </li>
							</ul>
						</div>
						<div class="col-lg-2  col-md-2 col-sm-4 col-xs-6">
							<h4>HELP </h4>
							<ul>
								<li> <a href="#"> Contact Us </a> </li>
								<li> <a href="#"> Track Orders </a> </li>
								<li> <a href="#"> FAQ'S </a> </li>
								<li> <a href="#"> Shipping Info </a> </li>
							</ul>
						</div>
						<div class="col-lg-2  col-md-2 col-sm-4 col-xs-6">
							<h4> QUICK LINKS</h4>
							<ul>
								<li> <a href="#"> Gift Cards </a> </li>
								<li> <a href="#"> Size Guide</a> </li>
								<li> <a href="#"> My Account</a> </li>
								<li> <a href="#"> Store Locator </a> </li>
							</ul>
						</div>
						<div class="col-lg-2  col-md-2 col-sm-4 col-xs-6 ">
							<h4> FIND US ON </h4>
							<ul class="social">
								<li> <a href="#"> <i class=" fa fa-facebook">   </i> </a> </li>
								<li> <a href="#"> <i class="fa fa-twitter">   </i> </a> </li>
								<li> <a href="#"> <i class="fa fa-google-plus">   </i> </a> </li>
								<li> <a href="#"> <i class="fa fa-pinterest">   </i> </a> </li>
								<li> <a href="#"> <i class="fa fa-youtube">   </i> </a> </li>
							</ul>
						</div>
						<div class="col-lg-3  col-md-3 col-sm-6 col-xs-12 ">
							<h4> Sign Up for special promotions </h4>
							<ul>
								<li>
									<div class="input-append newsletter-box text-center">
										<input type="text" class="full text-center" placeholder="Email ">
										<button class="btn  bg-gray" type="button"> Sign UP <i class="fa fa-long-arrow-right"> </i> </button>
									</div>
								</li>
							</ul>
						</div>
					</div>
					<!--/.row--> 
				</div>
				<!--/.container--> 
			</div>
			<!--/.footer-->
			<div class="footer-bottom">
				<div class="container">
					<p>Copyright &copy; <a href="home.php">www.bling.com</a> 2014</p>
					<div class="pull-right">
						<ul class="nav nav-pills payments">
							<li><i class="fa fa-cc-visa"></i></li>
							<li><i class="fa fa-cc-mastercard"></i></li>
							<li><i class="fa fa-cc-amex"></i></li>
							<li><i class="fa fa-cc-paypal"></i></li>
						</ul>
					</div>
				</div>
			</div>
			<!--/.footer-bottom--> 
		</div>
		<!-- /.container -->
		<!-- jQuery -->
		<script src="js/jquery.js"></script>
		<!-- Bootstrap Core JavaScript -->
		<script src="js/bootstrap.min.js"></script>
	</body>
</html>