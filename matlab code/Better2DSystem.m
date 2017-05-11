
% % MYMEAN Example of a local function.
iC=[0,0,0];
iR=2;

oC=[1,0,0];
oR=5;


oCirc=circle2(0,0,oR,oC);
iCirc=circle2(0,0,iR,iC);


totFrames=200;
t=linspace(0,10,totFrames)';
A=2;
gait=[A*sin(t),A/2*sin(t)*A.*cos(2*t)];
v = VideoWriter('2dSmart.mp4','MPEG-4');
open(v);
for i=1:totFrames
clf('reset');
axis equal
axis([-20,20,-20,20]);

oCirc=circle2(gait(i,1),gait(i,2),oR,oC);
vert=rectangle('Position',[-0.5,gait(i,2)-2.25*iR,1, iR*4.5],'FaceColor',iC);
horz=rectangle('Position',[-2.25*iR+gait(i,1),-0.5, iR*4.5, 1],'FaceColor',iC);
iCirc=circle2(0,0,iR,iC);
f=getframe;
writeVideo(v,f);
end
close(v);
function h = circle2(x,y,r,c)
d = r*2;
px = x-r;
py = y-r;
h = rectangle('Position',[px py d d],'Curvature',[1,1]);
h.FaceColor=c;
end
function h = rect2(x,y,r,c)
d = r*2;
px = x-r;
py = y-r;
h = rectangle('Position',[px py 2 4]);
h.FaceColor=c;
end