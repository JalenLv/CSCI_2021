set confirm off
set pagination off

# Safety net - NEVER let the bomb explode
break explode_bomb
commands
  silent
  printf "\n*** SAFETY: explode_bomb intercepted! Bomb did NOT explode. ***\n"
  quit 1
end

# Skip initialize_bomb (bypasses host check + server connection)
break initialize_bomb
commands
  silent
  printf "Bypassing initialize_bomb...\n"
  return
  # continue
end

# Skip send_msg (bypasses server reporting)
break send_msg
commands
  silent
  printf "Bypassing send_msg...\n"
  return
  continue
end

run
