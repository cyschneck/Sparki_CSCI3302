# Sparki Readme

## Reference Material

### Controls

### Functions
See sparki.h

### Kinematics
(x dot R)=x_r'=(r(dot)phi_l)/2+(r(dot)phi_r)/2  -> r = radius of wheels, dot=multiplication, phi=wheel speeds
(y dot R)=y_r'=0
(theta dot R)=theta_r'=(r(dot)phi_r)/d - (r(dot)phi_r)/d  -> r=radius, dot = multiplication, phi=wheel speeds, d=distance between axles

### Inverse Kinematics
a,b,c are "Controller Gains"

1. Calculate Error: rho=sqrt((x_start-x_goal)^2+(y_start-y_goal)^2)
                    alpha=theta_start+arctan((y_goal-y_start)/(x_goal-x_start))
                    nu=theta_goal-theta_start
2. Find the robot speed: x_r'=a(dot)rho
                         theta_r'=b(dot)alpha+c(dot)nu
3. Calculate the wheel speed: phi_l = (2(x_r'/r)-theta'(dot)(d))/2r
                              phi_r = (2(x_r'/r)+theta'(dot)(d))/2r
