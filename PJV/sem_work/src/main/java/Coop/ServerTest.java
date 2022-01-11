package Coop;

import org.junit.Assert;
import org.junit.Test;

import java.io.*;
import java.net.Socket;

/**
 * Makes the test connection to the server.
 * @author timusfed
 */
public class ServerTest {

    /**
     * Feedback test.
     * @throws Exception The exception will be thrown if the server is offline, or the client doesn't connect.
     */
    @Test
    public void testClientCall() throws Exception {
        Socket socket = new Socket("localhost", Client.PORT);
        PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        out.println("ping");
        Assert.assertEquals("pong", in.readLine());
        out.close();
        in.close();
        socket.close();
    }
}
