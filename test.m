clc
A = randi(56,8,7) % reshape(randperm(56),8,7) %
A = [sort(A(1,:));
     sort(A(2,:),'descend');
     sort(A(3,:));
     sort(A(4,:),'descend');
     sort(A(5,:));
     sort(A(6,:),'descend');
     sort(A(7,:));
     sort(A(8,:),'descend');]
A = [min(A(1:2,:));
     max(A(1:2,:));
     max(A(3:4,:));
     min(A(3:4,:));
     min(A(5:6,:));
     max(A(5:6,:));
     max(A(7:8,:));
     min(A(7:8,:))]
A = [sort(A(1,:));
     sort(A(2,:));
     sort(A(3,:),'descend');
     sort(A(4,:),'descend');
     sort(A(5,:));
     sort(A(6,:));
     sort(A(7,:),'descend');
     sort(A(8,:),'descend')]
A = [min(A([1,3],:));
     min(A([2,4],:));
     max(A([1,3],:));
     max(A([2,4],:));
     max(A([5,7],:));
     max(A([6,8],:));
     min(A([5,7],:));
     min(A([6,8],:))]
A = [min(A(1:2,:));
     max(A(1:2,:));
     min(A(3:4,:));
     max(A(3:4,:));
     max(A(5:6,:));
     min(A(5:6,:));
     max(A(7:8,:));
     min(A(7:8,:))]
A = [sort(A(1,:));
     sort(A(2,:));
     sort(A(3,:));
     sort(A(4,:));
     sort(A(5,:),'descend');
     sort(A(6,:),'descend');
     sort(A(7,:),'descend');
     sort(A(8,:),'descend')]
% last
A = [min(A([1,5],:));
     min(A([2,6],:));
     min(A([3,7],:));
     min(A([4,8],:));
     max(A([1,5],:));
     max(A([2,6],:));
     max(A([3,7],:));
     max(A([4,8],:))]
A = [min(A([1,3],:));
     min(A([2,4],:));
     max(A([1,3],:));
     max(A([2,4],:));
     min(A([5,7],:));
     min(A([6,8],:));
     max(A([5,7],:));
     max(A([6,8],:))]
A = [min(A(1:2,:));
     max(A(1:2,:));
     min(A(3:4,:));
     max(A(3:4,:));
     min(A(5:6,:));
     max(A(5:6,:));
     min(A(7:8,:));
     max(A(7:8,:))]
A = [sort(A(1,:));
     sort(A(2,:));
     sort(A(3,:));
     sort(A(4,:));
     sort(A(5,:));
     sort(A(6,:));
     sort(A(7,:));
     sort(A(8,:))]
