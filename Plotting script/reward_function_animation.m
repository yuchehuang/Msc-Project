figure(1)
filename = 'reward1.gif';
vol_concerntrated_factor= 0.01;
angular_concerntrated_factor= 0.2;

direction = [0 0 1];

for n=0.01:0.2:11
    [angle,velocity]=meshgrid(-3.14:0.01:3.14,-10:0.05:10);
    arg=(angle.^2)*(angular_concerntrated_factor+n);
    arg=arg+(velocity.^2*(vol_concerntrated_factor+n));
    reward=exp(-arg);
    C = reward;
    h=surf(angle,velocity,reward,C);

%% Figure layout configuration
    rotate(h,direction,0);
    set(h,'LineStyle','none');
    xlabel('angle');
    ylabel('velocity');
    zlabel('reward');
    colorbar
    drawnow
    frame = getframe(1);
    im = frame2im(frame);
    [imind,cm] = rgb2ind(im,256);
    
    frame = getframe(1);
    
%% Frame save    
    if n == 0.01;
          imwrite(imind,cm,filename,'gif', 'Loopcount',inf);
    else
          imwrite(imind,cm,filename,'gif','WriteMode','append');
    end
    
    
end