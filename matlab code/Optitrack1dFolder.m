clear all
fold=uigetdir('A:\1DSmartData\Optitrack\');
f=dir2(fullfile(fold,'*.csv'));
%     clf;
movs=struct;
nMovs=length(f);
movs(nMovs).fname='';
dec=1; %decimate amount
%HANDEDNESS IN QUATERNIONS ISNT CHANGED?
conv=zeros(nMovs,1);

closeWaitbar;
fold
h = waitbar(0,'Please wait...');
    steps = nMovs;
    

for i=1:nMovs

% for i =1:length(f)
    waitbar(i/steps,h,{['Processing: ',num2str(i),'/',num2str(length(f))],f(i).name})
    pts(i,'/',nMovs, ' ',fullfile(fold,f(i).name));
%     [t,x,y,tracks]
    [movs(i).t,movs(i).x,movs(i).y]= Optitrack1d(fullfile(fold,f(i).name),dec);
    
    [yvals inds]=sort(movs(i).y(end,:),'ascend');
    frame=inds(1);smarts=[inds(5),inds(2)]; rots=[inds(4),inds(3)];
    movs(i).rots=[movs(i).x(:,rots) movs(i).y(:,rots)];
    movs(i).frame=[movs(i).x(:,frame),movs(i).y(:,frame)];
    movs(i).smarts=[movs(i).x(:,smarts),movs(i).y(:,smarts)];
    movs(i).fname=f(i).name;
    movs(i).fps=120/dec;
    movs(i).conv=1;
    [~,vals]=parseFileNames(f(i).name);
    %%
%     spk=[0]; smart=[-90]; gait=[1]; rob=[5]; v=[nMovs];
%     vals=[0 0 1 5 i];
    %%
    movs(i).pars=vals;
    
    
%     pause(1);

end
closeWaitbar;
save(fullfile(fold,'movieInfo.mat'),'movs','fold','nMovs')
