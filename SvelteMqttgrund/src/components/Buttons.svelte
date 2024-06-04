<script>
    	import { onSend, startConnect} from '../stores/mqttStore';
    	import { isConnected} from '../stores/store';

    // Function to send MQTT message
    function sendMessage(direction) {
        if (!['up', 'down', 'right', 'left','stop', 'straight' ].includes(direction))
            return;

        // Publish the message to MQTT topic
        onSend('Driverbot', direction);
        
    }
</script>

<style>
</style>
<div class="flex justify-center bg-slate-100 space-x-4">
   
    <div class="flex flex-col items-center space-y-2 bg-white p-4 rounded shadow">
        {#if $isConnected}
        <img src="https://th.bing.com/th/id/OIG4.K8D8S1C4kMKieVykgK2H?w=1024&h=1024&rs=1&pid=ImgDetMain">
        <p class="text-green-500">Connected</p>
    {:else}
        <p class="text-red-500">Not Connected</p>
    {/if}
        <!-- Top Buttons -->
        <div><button class="btn variant-filled" on:click={() => sendMessage('up')}>
            Forward</button>
        <button class="btn variant-filled" on:click={() => sendMessage('stop')}>
            Stop</button>
    
        <button class="btn variant-filled" on:click={() => sendMessage('straight')}>
            Straight</button></div>
        
        <!-- Middle Row of Buttons -->
        <div class="flex justify-between space-x-2 w-full">
            <button class="btn variant-filled" on:click={() => sendMessage('left')}>
                Left
            </button>
            <button class="btn variant-outline-secondary bg-cyan-300" disabled={$isConnected} on:click={startConnect}>
                Connect
            </button>
          <button class="btn variant-filled" on:click={() => sendMessage('right')}>
            Right
        </button>
        </div>
        
        <!-- Bottom Button -->
        <button class="btn variant-filled" on:click={() => sendMessage('down')}>
            Backward
        </button>
      </div>
</div>
