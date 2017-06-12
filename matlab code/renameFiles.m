% fold=uigetdir('A:\1DSmartData\Optitrack\');
fold=uigetdir('A:\2DSmartData\Session 2017-06-06');
% A:\1DSmartData\Optitrack\Session 2017-04-24\inactive
f=dir2(fullfile(fold,'*.csv'));
% [~,num]=parseFileNames(fold)
nMovs=length(f);
s=1; pa=1; pb=0; m=0; foam=0;
% m=0 is joint
% m=1 is wheel ccw for smart a
% m=2 is wheel cw for smart a
% m=-1 is wheel ccw for smart b
% m=-2 is wheel cw for smart b

% f=0 no foam
% f=-1 smart a
% f= 1 smart b

%s=10 is for test cases

for i=1:nMovs
%     fnew=['s=',num2str(s),'_pa=',num2str(pa),'_pb=',num2str(pb),'_v=',num2str(i),'_m=',num2str(m),'_f=',num2str(foam),'.csv'];
    fnew=['s=',num2str(s),'_pa=',num2str(pa),'_pb=',num2str(pb),'_v=',num2str(i),'.csv'];
    copyfile(fullfile(fold,f(i).name),fullfile(fold,fnew));
end