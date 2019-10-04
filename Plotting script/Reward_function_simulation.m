clear;
vol_concerntrated_factor= 0.01;
angular_concerntrated_factor= 0.2;
[angle,velocity]=meshgrid(-3.14:0.01:3.14,-10:0.05:10);
arg=(angle.^2)*angular_concerntrated_factor;
arg=arg+(velocity.^2*vol_concerntrated_factor);
reward=exp(-arg);
C = reward;
figure
h=surf(angle,velocity,reward,C);
set(h,'LineStyle','none');
xlabel('angle');
ylabel('velocity');
zlabel('reward');
 colorbar

%reward=exp(((3.14^2)-angle.^2/angular_concerntrated_factor)-(velocity.^2/vol_concerntrated_factor));
