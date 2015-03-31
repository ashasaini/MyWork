/*
	1.00	Y.Ohiwa		??.??.????	original.
	1.01	Y.Ohiwa		29.Sep.1999	for octane.
*/


#define	BDM_CTL_INIT	1
#define	BDM_CTL_LED	2
#define	BDM_CTL_BUTTON	3
#define	BDM_CTL_DIAL	4
#define	BDM_CTL_DEBUG	5
#define	BDM_CTL_KEY	6
#define	BDM_CTL_INTR	7

typedef	struct {
	u_int	base;
	u_int	vector;
	u_int	ipl;
}bdm_ctl_init_t;

typedef	struct {
	int	mask;
	int	values;
}bdm_ctl_led_t;

typedef struct {
	int	type;
}bdm_ctl_button_t;

typedef struct {
	int	type;
	int	value;
}bdm_ctl_dial_t;

typedef struct {
	int	level;
}bdm_ctl_debug_t;

typedef struct {
	int	use;
	int	timeout;
}bdm_ctl_key_t;
