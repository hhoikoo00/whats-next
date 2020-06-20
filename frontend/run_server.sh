PYTHON_VER=$(python -c 'import sys; print(sys.version_info[0])')

if [[ $PYTHON_VER -eq 3 ]]; then
    echo "Python 3 detected..."
    echo "Running server"
    python -m http.server
elif [[ $PYTHON_VER -eq 2 ]]; then
    echo "Python 2 detected..."
    echo "Running server"
    python -m SimpleHTTPServer
fi
