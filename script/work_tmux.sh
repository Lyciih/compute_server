#! /bin/bash
work_SESSION="compute_server"



if tmux has-session -t work_SESSION 2>/dev/null; then
	echo "session 已存在"
else

	tmux new-session -d -s $work_SESSION
	tmux send-keys -t $work_SESSION:0 'cd ..&&clear' C-m 

	tmux split-window -h -p 50 -t $work_SESSION:0
	tmux send-keys -t $work_SESSION:0 'cd ..&&clear' C-m 
fi
tmux attach -t $work_SESSION:0




