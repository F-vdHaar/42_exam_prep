 See 
 attachment/subject.en.txt    
    
    
      ┌────────────┐
      │ static buf │ <─── leftovers from last call
      └────┬───────┘
           │
           ▼
   ┌────────────────────┐
   │ ft_strdup(buf)     │   ← start line with leftovers
   └────────────────────┘
           │
   ┌───────▼────────┐
   │ while no '\n'  │
   │ and read > 0   │
   └───────┬────────┘
           ▼
 ┌────────────────────────────┐
 │ read from fd into buf      │
 │ buf[r] = '\0'              │
 │ line = ft_strjoin_free()   │
 └────────────────────────────┘
           │
   ┌───────▼─────────┐
   │ newline found?  │───Yes──┐
   └───────┬─────────┘        │
           ▼                 ▼
    ┌──────────────┐  ┌──────────────────────────┐
    │ Check errors │  │ extract line + newline   │
    │ or EOF/empty │  │ copy leftovers to buffer │
    └──────────────┘  └──────────────────────────┘
                             │
                             ▼
                        return result
