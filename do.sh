if [$1 == "all"] then
  echo "all"
  touch *.*;
  make makefile all;
else
  echo "none"
fi

