function [t,x,y]=trackOptitrack(file,dec)

% figure(1);clf;

% files = dir([folder '*.csv']);
% tMod = [files(:).datenum];

% [~,ind] = max(tMod);
%rx = rigid body
%frame, time, qx, qy, qz, qw, rigX, rigY, rigZ
data = importdata(file);
%recorded frame->desired frame
%x->-x
%z->y
%y->z


% %QUATERNION X DATA MIGHT NEED TO BE FLIPPED!!
% % if (data.textdata{6,6}=='W') %system has a rigid body
% % q=[data.data(:,6) data.data(:,3) data.data(:,5) data.data(:,4)];
% % q=quaternion(q'); %I need to transpose it before setting to quat for matrix
% % angles = EulerAngles(q,'123');
% % ang=reshape(angles(3,1,:),[size(angles,3),1]);
% 
% %decimate by dec
% t = data.data(:,2); 
% x = -data.data(:,7);
% y = data.data(:,9);
% z = data.data(:,8);
% 
% 
% t=t(1:dec:end,:);
% comx=x(1:dec:end,:);
% comy=y(1:dec:end,:);
% % comz=z(1:dec:end,:);
% ang=ang(1:dec:end,:);
% % pts('rigid body sys');
% varargout{1}=ang;
% else
% % -repmat(data.data(1,3:3:end),[size(data.data(:,3:3:end),1),1])
data.data(any(isnan(data.data),2),:)=[];    
t = data.data(:,2);
x = -data.data(:,3:3:end)*1000;
y = data.data(:,5:3:end)*1000;
% z = data.data(:,4:3:end);

%decimate by dec
t=t(1:dec:end,:);
x=x(1:dec:end,:);
y=y(1:dec:end,:);
% z=z(1:dec:end,:);


% COM=[t,x,y,z];
% COM(any(isnan(COM),2),:)=[];
% 
% tracks=cell(1);
% tracks{1}= COM;
