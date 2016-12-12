%PLOT_OFFLINE.M
%Plotting constants
HEADING_LINE = 50; %[mm]
%Runtime variables
robot_path = []; % will store all the x_pos and y_pos positions
%Create a new figure a define the plot handlers for the robot path and heading
figure;
hold on;grid on;axis equal;
plt_robot_path = plot(0,0,'Color',[0,.5,0]);
plt_heading = line([0,0],[0,0],'Color','k','LineWidth',3);
xlabel('X [mm]');
ylabel('Y [mm]');
%Drawing path loop
system(' grep "X:" data.log | sed s/.*POSITION.*://g > data.tmp');
fid = fopen('data.tmp', 'r');
while (1)
	%Read data from file
	log_line = fgetl(fid);
	if(log_line== -1) 
		break;
	end;
	[data] = sscanf(log_line,' %f ', [1,5]);
	x_pos = data(end,1);
	y_pos = data(end,2);
	heading = data(end,3) * pi/180;
	%Update path
	robot_path = [robot_path; [x_pos, y_pos]];
	set(plt_robot_path, 'XData', robot_path(:,1), 'YData', robot_path(:,2));
	set(plt_heading, 'XData', [x_pos, x_pos + HEADING_LINE * cos(heading)], 'YData', [y_pos,y_pos + HEADING_LINE * sin(heading)]);
	drawnow;
end
fclose(fid);